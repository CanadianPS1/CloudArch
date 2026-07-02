#include <iostream>
#include <chrono>
#include <string>
#include "include/nlohmann/json.hpp"
namespace ClassWork{
    class OneThree{
        nlohmann::json LambdaHandler(nlohmann::json event){
            std::string requiredFields[] = {"title", "director_name", "runtime_minutes", "release_year"};
            for(std::string field : requiredFields){
                if(!event.contains(field)){
                    nlohmann::json result = {
                        {"statusCode", 400},
                        {"body",
                            {"message", "Missing required fields : " + field}
                        }
                    };
                    return result;
                }
            }
            float runtimeMinutes = event.value("runtime_minutes", 0);
            if(runtimeMinutes <= 0){
                nlohmann::json result = {
                        {"statusCode", 400},
                        {"body",
                            {"message", "runtime minutes must be a positive integer"}
                        }
                    };
                    return result;
            }
            const std::string title = event.value("title", "");
            const std::string directorName = event.value("director_name", "");
            const int releaseyear = event.value("release_year", 0);
            const auto now = std::chrono::system_clock::now();
            const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
            //const currentYear = 
            nlohmann::json result = {
                {"echo", inputText},
                {"reversed", inputText},
                {"length", inputText.size()},
                {"timestamp", std::ctime(&t_c)}
            };
            return result;
        }
        public:
            void RunLocalTests(){
                std::cout<<"Hello World"<<std::endl;
                nlohmann::json test = {
                    {"inputText", "Hello AWS!"},
                    {"inputText", "GoodBye AWS!"},
                    {"inputText", "x"},
                    {}
                };
                for(auto event : test){
                    nlohmann::json result = LambdaHandler(test);
                    std::cout<<"Input : "<<event<<std::endl;
                    std::cout<<"Result : "<<result<<std::endl<<std::endl;
                }
            }
    };
};
int main(){
    ClassWork::OneThree* test = new ClassWork::OneThree();
    test->RunLocalTests();
    delete test;
    return 0;
}