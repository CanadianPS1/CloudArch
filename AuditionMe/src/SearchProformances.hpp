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
            std::string live = "";
            if(isLive) live = "true";
            else live = "false";
            return "{{\"title\", \"" + title + "\"},{\"director\", \"" + director + "\"},{\"venue\", \"" + venue + "\"},{\"isLive\",\"" + live +"\"},{\"castingDirector\",\"" + castingDirector +"\"},{\"characters\",\"" + characters +"\"},{\"performanceDates\",\"" + performanceDates +"\"}}";
        }
    };
    static std::vector<Proformance> proformances;
    struct Proformances{
        nlohmann::json static GetProformances(){
            //temp thing for the test stuff
            if(proformances.empty()) SetProformances();
            if(proformances.empty()) return {};
            std::string fullString = "";
            for(Proformance proformance : proformances) fullString += proformance.ToString();
            nlohmann::json res = fullString;
            return res;
        }
        private:
            void static SetProformances(){
                Aws::SDKOptions options;
                Aws::InitAPI(options);{
                    Aws::Client::ClientConfiguration config;
                    config.region = "us-east-2";
                    Aws::DynamoDB::DynamoDBClient client(config);
                    Aws::DynamoDB::Model::ScanRequest request;
                    request.SetTableName("Proformances");
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