#include "../include/nlohmann/json.hpp"
#include "chrono"
#include <uuid/uuid.h>
#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/AttributeValue.h>
namespace AuditionMe{
    struct CreateProformance{
        int static Checker(nlohmann::json req){
            if(!req.contains("title") || !req.contains("director") || !req.contains("castingDirector") || !req.contains("venue") || 
                !req.contains("performanceDates") || !req.contains("characters") || !req.contains("isLive")) return 1;
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
            if(correctReq != 200) return res = {
                {"statusCode", 400},
                {"message", "your request didnt contain all required fields"},
                {"timeStamp",std::ctime(&t_c)}
            };
            std::string uuid = GetUuid();
            Aws::SDKOptions options;
            Aws::InitAPI(options);{
                Aws::Client::ClientConfiguration config;
                config.region = "us-east-2";
                Aws::DynamoDB::DynamoDBClient client(config);
                Aws::DynamoDB::Model::PutItemRequest request;
                Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> item;
                Aws::DynamoDB::Model::AttributeValue id;
                Aws::DynamoDB::Model::AttributeValue title;
                Aws::DynamoDB::Model::AttributeValue director;
                Aws::DynamoDB::Model::AttributeValue castingDirector;
                Aws::DynamoDB::Model::AttributeValue venue;
                Aws::DynamoDB::Model::AttributeValue performanceDates;
                Aws::DynamoDB::Model::AttributeValue characters;
                Aws::DynamoDB::Model::AttributeValue isLive;
                request.SetTableName("Proformances");
                title.SetS(req["title"]);
                director.SetS(req["director"]);
                castingDirector.SetS(req["castingDirector"]);
                venue.SetS(req["venue"]);
                performanceDates.SetS(req["performanceDates"]);
                characters.SetS(req["characters"]);
                isLive.SetBool(req["isLive"]);
                id.SetS(uuid);
                item["id"] = id;
                item["title"] = title;
                item["director"] = director;
                item["castingDirector"] = castingDirector;
                item["venue"] = venue;
                item["performanceDates"] = performanceDates;
                item["characters"] = characters;
                item["isLive"] = isLive;
                request.SetItem(item);
                auto outcome = client.PutItem(request);
            }Aws::ShutdownAPI(options);
            res = {
                {"statusCode", 200},
                {"id", uuid},
                {"title", req["title"]},
                {"director", req["director"]},
                {"castingDirector", req["castingDirector"]},
                {"venue", req["venue"]},
                {"performanceDates", req["performanceDates"]},
                {"characters", req["characters"]},
                {"isLive", req["isLive"]},
                {"created_at", std::ctime(&t_c)},
                {"message", "User created successfully"}
            };
            return res;
        }
    };
}