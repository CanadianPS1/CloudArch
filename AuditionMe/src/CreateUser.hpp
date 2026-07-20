#include "../include/nlohmann/json.hpp"
#include "chrono"
#include <string>
#include <uuid/uuid.h>
#include <regex>
#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/AttributeValue.h>
namespace AuditionMe{
    struct CreateUser{
        int static Checker(nlohmann::json req){
            if(!req.contains("name") || !req.contains("email") || !req.contains("phone") || !req.contains("role")) return 1;
            std::string name = req["name"], email = req["email"], phone = req["phone"], role = req["role"];
            std::regex emailRegex(R"(([\w\.-]+)@([\w\.-]+)\.([a-zA-Z]{2,}))");
            if(role.compare("performer") != 0 && role.compare("director") != 0) return 2;
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
            Aws::SDKOptions options;
            Aws::InitAPI(options);{
                Aws::Client::ClientConfiguration config;
                config.region = "us-east-2";
                Aws::DynamoDB::DynamoDBClient client(config);
                Aws::DynamoDB::Model::PutItemRequest request;
                Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> item;
                Aws::DynamoDB::Model::AttributeValue id;
                Aws::DynamoDB::Model::AttributeValue name;
                Aws::DynamoDB::Model::AttributeValue email;
                Aws::DynamoDB::Model::AttributeValue phone;
                Aws::DynamoDB::Model::AttributeValue role;
                request.SetTableName("Users");
                email.SetS(req["email"]);
                phone.SetS(req["phone"]);
                name.SetS(req["name"]);
                role.SetS(req["role"]);
                id.SetS(uuid);
                item["userId"] = id;
                item["name"] = name;
                item["email"] = email;
                item["phone"] = phone;
                item["role"] = role;
                request.SetItem(item);
                auto outcome = client.PutItem(request);
            }Aws::ShutdownAPI(options);
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