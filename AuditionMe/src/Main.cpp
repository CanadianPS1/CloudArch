#include "CreateUser.hpp"
#include "CreateProformance.hpp"
#include "SearchProformances.hpp"
#include "ListPerformances.hpp"
#include "Auth.hpp"
#include "../include/nlohmann/json.hpp"
#include <iostream>
namespace AuditionMe{
    int RunLocalTests(){
        nlohmann::json req = {
            {"name", "Lia"},
            {"email", "spielenermode@gmail.com"},
            {"phone", "814-824-9384"},
            {"role", "director"}
        };
        nlohmann::json res;
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 200) return 1;
        req = {
            {"name", "Lia"},
            {"email", "spielenermode@gmail.com"},
            {"phone", "814-824-9384"},
            {"role", "gamer"}
        };
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 400) return 2;
        req = {
            {"name", "Lia"},
            {"email", "spielenermodegmail.com"},
            {"phone", "814-824-9384"},
            {"role", "director"}
        };
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 400) return 3;
        req = {
            {"name", "Lia"},
            {"email", "spielenermodegmailcom"},
            {"phone", "814-824-9384"},
            {"role", "director"}
        };
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 400) return 4;
        req = {
            {"name", "Lia"},
            {"email", "spielenermode@gmail.com"},
            {"phone", },
        };
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 400) return 5;
        req = {
            {"name", "Lia"},
            {"email", "spielenermode@gmail.com"},
            {"phone", "814-824-9384"},
            {"role", "performer"}
        };
        res = CreateUser::Hitter(req);
        if(res["statusCode"] != 200) return 6;
        return -1;
    }
}
int main(){
    int testResult = AuditionMe::RunLocalTests();
    if(testResult != -1) std::cout<<"failed on test number: "<<testResult<<std::endl;
    else std::cout<<"all tests pass"<<std::endl;
    return 0;
}






// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/strand.hpp>
// #include <boost/config.hpp>
// #include "../include/nlohmann/json.hpp"
// #include <iostream>
// #include <memory>
// #include <chrono>
// #include <iterator>
// #include <regex>
// namespace beast = boost::beast;
// namespace http = boost::beast::http;
// namespace net = boost::asio;
// using tcp = net::ip::tcp;
// http::response<http::string_body> handle_request(http::request<http::string_body> const& req){
//     if(req.method() == http::verb::post && req.target() == ("/create")){
//         nlohmann::json body = nlohmann::json::parse(req.body());
//         std::string role = body["role"];
//         std::string name = body["name"];
//         std::string email = body["email"];
//         std::string phone = body["phone"];
//         if(role != "performer" || role != "director" || name == "" || email == "" || phone == ""){
//             nlohmann::json json_response = {{"message", "bad request, you had an empty input or a incorrect role"}};
//             http::response<http::string_body> res{http::status::bad_request, req.version()};
//             res.set(http::field::server, "Beast");
//             res.set(http::field::content_type, "application/json");
//             res.keep_alive(req.keep_alive());
//             res.body() = json_response.dump();
//             res.prepare_payload();
//             return res;
//         }
//         std::regex emailRegex("^[\\w\\-\\.]+@([\\w-]+\\.)+[\\w-]{2,}$");

//         nlohmann::json json_response = {{"message", "hello world"},
//                                         {"URL", "http://www.localhost:8082/create"}};
//         http::response<http::string_body> res{http::status::ok, req.version()};
//         res.set(http::field::server, "Beast");
//         res.set(http::field::content_type, "application/json");
//         res.keep_alive(req.keep_alive());
//         res.body() = json_response.dump();
//         res.prepare_payload();
//         auto end = std::chrono::system_clock::now();
//         return res;
//     }
//     return http::response<http::string_body>{http::status::bad_request, req.version()};
// }
// class Session : public std::enable_shared_from_this<Session>{
//     tcp::socket socket_;
//     beast::flat_buffer buffer_;
//     http::request<http::string_body> req_;
//     public:
//         explicit Session(tcp::socket socket) : socket_(std::move(socket)){}
//         void run(){do_read();}
//     private: 
//         void do_read(){
//             auto self(shared_from_this());
//             http::async_read(socket_,buffer_,req_, [this, self](beast::error_code ec, std::size_t){if(!ec) do_write(handle_request(req_));});
//         }
//         void do_write(http::response<http::string_body> res){
//             auto self(shared_from_this());
//             auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
//             http::async_write(socket_, *sp, [this,self,sp](beast::error_code ec, std::size_t){socket_.shutdown(tcp::socket::shutdown_send);});
//         }
// };
// class Listener : public std::enable_shared_from_this<Listener>{
//     net::io_context& ioc_;
//     tcp::acceptor acceptor_;
//     public:
//         Listener(net::io_context& ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(net::make_strand(ioc)){
//             beast::error_code ec;
//             acceptor_.open(endpoint.protocol());
//             acceptor_.set_option(net::socket_base::reuse_address(true));
//             acceptor_.bind(endpoint);
//             acceptor_.listen(net::socket_base::max_listen_connections);
//             do_accept();
//         }
//     private:
//         void do_accept(){
//             acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket){
//                 if(!ec) std::make_shared<Session>(std::move(socket))->run();
//                 do_accept();
//             });
//         };
// };
// int main(){
//     try{
//         auto const address = net::ip::make_address("0.0.0.0");
//         unsigned short port = 8081;
//         net::io_context ioc{1};
//         auto listener = std::make_shared<Listener>(ioc,tcp::endpoint{address,port});
//         std::cout<<"running on port: "<<std::to_string(port)<<std::endl;
//         ioc.run();
//     }catch(const std::exception& e){
//         std::cerr<<"Error: "<< e.what()<<std::endl;
//     }
// }