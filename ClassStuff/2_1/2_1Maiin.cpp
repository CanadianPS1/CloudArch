#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include "include/nlohmann/json.hpp"
#include <iostream>
#include <memory>
#include <chrono>
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
http::response<http::string_body> handle_request(http::request<http::string_body> const& req){
    if(req.method() == http::verb::get && req.target().starts_with("/hello-")){
        std::string target = std::string(req.target());
        target = target.substr(0, target.find('?'));
        if(!target.empty() && target.back() == '/') target.pop_back();
        std::string userName = target.substr(target.find_last_of('-') + 1);
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        nlohmann::json json_response = {{"message",  "Hello " + userName + "!!!"},
                                        {"name", userName},
                                        {"timeStamp", std::ctime(&t_c)},
                                        {"URL", "http://www.localhost:8081/hello-" + userName}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
class Session : public std::enable_shared_from_this<Session>{
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    public:
        explicit Session(tcp::socket socket) : socket_(std::move(socket)){}
        void run(){do_read();}
    private: 
        void do_read(){
            auto self(shared_from_this());
            http::async_read(socket_,buffer_,req_, [this, self](beast::error_code ec, std::size_t){if(!ec) do_write(handle_request(req_));});
        }
        void do_write(http::response<http::string_body> res){
            auto self(shared_from_this());
            auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
            http::async_write(socket_, *sp, [this,self,sp](beast::error_code ec, std::size_t){
                socket_.shutdown(tcp::socket::shutdown_send);
            });
        }
};
class Listener : public std::enable_shared_from_this<Listener>{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    public:
        Listener(net::io_context& ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(net::make_strand(ioc)){
            beast::error_code ec;
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(net::socket_base::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen(net::socket_base::max_listen_connections);
            do_accept();
        }
    private:
        void do_accept(){
            acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket){
                if(!ec) std::make_shared<Session>(std::move(socket))->run();
                do_accept();
            });
        };
};
void RunLocalTests(){
    /*
        http method
        path
        headers
        queryStringparameters
        body
        pathparameters
        request contexted
    */
}
int main(){
    try{
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 8081;
        net::io_context ioc{1};
        auto listener = std::make_shared<Listener>(ioc,tcp::endpoint{address,port});
        ioc.run();
    }catch(const std::exception& e){
        std::cerr<<"Error: "<< e.what()<<std::endl;
    }
}