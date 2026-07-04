#include <iostream>
#include <chrono>
#include <string>
#include "include/nlohmann/json.hpp"
namespace ClassWork{
    class OneThree{
        nlohmann::json LambdaHandler(nlohmann::json event){
            std::string inputText = event.value("input_text", "");
             const auto now = std::chrono::system_clock::now();
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
            nlohmann::json result = {
                {"echo", inputText},
                {"reversed", inputText},
                {"length", inputText.size()},
                {"timestamp", std::ctime(&t_c)}
            };
            return result;
        }
        public:
            // void RunLocalTests(){
            //     std::cout<<"Hello World"<<std::endl;
            //     nlohmann::json test = {
            //         {"inputText", "Hello AWS!"},
            //         {"inputText", "GoodBye AWS!"},
            //         {"inputText", "x"},
            //         {}
            //     };
            //     for(auto event : test){
            //         nlohmann::json result = LambdaHandler(test);
            //         std::cout<<"Input : "<<event<<std::endl;
            //         std::cout<<"Result : "<<result<<std::endl<<std::endl;
            //     }
            // }
    };
};
int main(){
    ClassWork::OneThree* test = new ClassWork::OneThree();
    //test->RunLocalTests();
    delete test;
    return 0;
}