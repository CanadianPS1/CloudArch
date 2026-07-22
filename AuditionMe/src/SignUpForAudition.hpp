#pragma once
#include "../include/nlohmann/json.hpp"
#include "chrono"
#include <string>
#include <uuid/uuid.h>
#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/AttributeValue.h>
namespace AuditionMe{
    struct SignUp{
        int static Checker(nlohmann::json req){
            if(!req.contains("performanceId") || !req.contains("performerId") || !req.contains("characterName")) return 1;
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
                {"timeStamp", std::ctime(&t_c)}
            };
            std::string uuid = GetUuid();
            Aws::SDKOptions options1;
            Aws::InitAPI(options1);{
                Aws::Client::ClientConfiguration config;
                config.region = "us-east-2";
                Aws::DynamoDB::DynamoDBClient client(config);
                Aws::DynamoDB::Model::ScanRequest request;
                request.SetTableName("Proformances");
                request.SetFilterExpression("id = :live");
                Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> values;
                Aws::DynamoDB::Model::AttributeValue live;
                live.SetS(req["performanceId"]);
                values[":live"] = live;
                auto outcome = client.Scan(request);
                const auto& res = outcome.GetResult().GetItems();
                if(res.size() >= 0) return {
                    {"statusCode", 400},
                    {"message", "your request didnt contain a valid preformance id"},
                    {"timeStamp", std::ctime(&t_c)}
                };
            }
            Aws::ShutdownAPI(options1);
            Aws::SDKOptions options2;
            Aws::InitAPI(options2);{
                Aws::Client::ClientConfiguration config;
                config.region = "us-east-2";
                Aws::DynamoDB::DynamoDBClient client(config);
                Aws::DynamoDB::Model::ScanRequest request;
                request.SetTableName("Users");
                request.SetFilterExpression("id = :live");
                Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> values;
                Aws::DynamoDB::Model::AttributeValue live;
                live.SetS(req["performerId"]);
                values[":live"] = live;
                auto outcome = client.Scan(request);
                const auto& res = outcome.GetResult().GetItems();
                if(res.size() >= 0) return {
                    {"statusCode", 400},
                    {"message", "your request didnt contain a valid preformer id"},
                    {"timeStamp", std::ctime(&t_c)}
                };
            }
            Aws::ShutdownAPI(options1);
            Aws::SDKOptions options;
            Aws::InitAPI(options);{
                Aws::Client::ClientConfiguration config;
                config.region = "us-east-2";
                Aws::DynamoDB::DynamoDBClient client(config);
                Aws::DynamoDB::Model::PutItemRequest request;
                Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> item;
                Aws::DynamoDB::Model::AttributeValue id;
                Aws::DynamoDB::Model::AttributeValue performanceId;
                Aws::DynamoDB::Model::AttributeValue performerId;
                Aws::DynamoDB::Model::AttributeValue characterName;
                Aws::DynamoDB::Model::AttributeValue status;
                request.SetTableName("Auditions");
                performanceId.SetS(req["performanceId"]);
                performerId.SetS(req["performerId"]);
                characterName.SetS(req["characterName"]);
                status.SetS("pending");
                id.SetS(uuid);
                item["userId"] = id;
                item["performanceId"] = performanceId;
                item["performerId"] = performerId;
                item["characterName"] = characterName;
                item["status"] = status;
                request.SetItem(item);
                auto outcome = client.PutItem(request);
            }Aws::ShutdownAPI(options);
            res = {
                {"statusCode", 200},
                {"id", uuid},
                {"performanceId", req["performanceId"]},
                {"performerId", req["performerId"]},
                {"characterName", req["characterName"]},
                {"created_at", std::ctime(&t_c)},
                {"message", "User created successfully"}
            };
            return res;
        }
    };
}