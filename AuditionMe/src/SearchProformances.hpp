#pragma once
#include <string>
#include <vector>
#include "../include/nlohmann/json.hpp"
#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/ScanRequest.h>
namespace AuditionMe{
    struct Proformance{
        std::string title, director, venue, castingDirector, performanceDates, characters;
        bool isLive;
        std::string ToString(){
            return nlohmann::json{
                {"title", title},
                {"director", director},
                {"venue", venue},
                {"isLive", isLive},
                {"castingDirector", castingDirector},
                {"characters", characters},
                {"performanceDates", performanceDates}
            }.dump();
        }
    };
    static std::vector<Proformance> proformances;
    struct Proformances{
        nlohmann::json static GetProformances(const bool liveQueryString){
            if(proformances.empty()) SetProformances(liveQueryString);
            if(proformances.empty()) return {};
            std::string fullString = "";
            for(Proformance proformance : proformances) fullString += proformance.ToString();
            nlohmann::json res = fullString;
            return res;
        }
        private:
            void static SetProformances(const bool liveQueryString){
                Aws::SDKOptions options;
                Aws::InitAPI(options);{
                    Aws::Client::ClientConfiguration config;
                    config.region = "us-east-2";
                    Aws::DynamoDB::DynamoDBClient client(config);
                    Aws::DynamoDB::Model::ScanRequest request;
                    request.SetTableName("Proformances");
                    request.SetFilterExpression("isLive = :live");
                    Aws::Map<Aws::String,Aws::DynamoDB::Model::AttributeValue> values;
                    Aws::DynamoDB::Model::AttributeValue live;
                    if(liveQueryString) live.SetBool(true);
                    else live.SetBool(false);
                    values[":live"] = live;
                    request.SetExpressionAttributeValues(values);
                    do{
                        auto outcome = client.Scan(request);
                        const auto& res = outcome.GetResult().GetItems();
                        Proformance proformance;
                        for(const auto& item : res){
                            proformance.title = item.at("title").GetS();
                            proformance.director = item.at("director").GetS();
                            proformance.castingDirector = item.at("castingDirector").GetS();
                            proformance.venue = item.at("venue").GetS();
                            proformance.isLive = item.at("isLive").GetBool();
                            proformance.performanceDates = item.at("performanceDates").GetS();
                            proformance.characters = item.at("characters").GetS();
                            proformances.push_back(proformance);
                        }
                        request.SetExclusiveStartKey(outcome.GetResult().GetLastEvaluatedKey());
                    }while(!request.GetExclusiveStartKey().empty());
                }
                Aws::ShutdownAPI(options);
            }
    };
}