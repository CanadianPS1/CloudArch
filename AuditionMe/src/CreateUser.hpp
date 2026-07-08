#include "../include/nlohmann/json.hpp"
#include "chrono"
#include <string>
#include <uuid/uuid.h>
#include <regex>
namespace AuditionMe{
    struct CreateUser{
        int static Checker(nlohmann::json req){
            std::string name = req["name"], email = req["email"], phone = req["phone"], role = req["role"];
            if(name == "" || email == "" || phone == "" || role == "") return 1;
            else if(role != "performer" || role != "director") return 2;
            std::regex emailRegex(R"(([\w\.-]+)@([\w\.-]+)\.([a-zA-Z]{2,}))");
            if(!std::regex_match(email, emailRegex)) return 3;
            return 200;
        }
        std::string static GetUuid(){
            uuid_t uuid;
            char uuid_str[37];
            uuid_generate_random(uuid);
            uuid_unparse_lower(uuid, uuid_str);
            return uuid_str;
        }
        nlohmann::json static Hitter(nlohmann::json req){
            nlohmann::json res;
            const auto now=std::chrono::system_clock::now();
    		const std::time_t t_c=std::chrono::system_clock::to_time_t(now);
            int correctReq = Checker(req);
            if(correctReq != 200){
                if(correctReq == 1) return res = {
                    {"statusCode", 400},
                    {"message", "your request didnt contain all required fields"},
 			        {"timeStamp",std::ctime(&t_c)}
                };
                else if(correctReq == 2) return res = {
                    {"statusCode", 400},
                    {"message", "your request had an unknown role"},
 			        {"timeStamp",std::ctime(&t_c)}
                };
                else if(correctReq == 3) return res = {
                    {"statusCode", 400},
                    {"message", "your request had an incorrect email"},
 			        {"timeStamp",std::ctime(&t_c)}
                };
            }
            std::string uuid = GetUuid();
            std::string name = req["name"], email = req["email"], phone = req["phone"], role = req["role"];
            res = {
                {"statusCode", 200},
                {"id", uuid},
                {"name", name},
                {"email", email},
                {"phone", phone},
                {"role", role},
                {"created_at", std::ctime(&t_c)},
                {"message", "User created successfully"}
            };
            return res;
        }
        
    };
}