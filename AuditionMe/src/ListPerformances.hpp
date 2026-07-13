#include <string>
#include <vector>
#include "../include/nlohmann/json.hpp"
namespace AuditionMe{
    struct Proformance{
        std::string title, director, venue;
        bool isLive;
        std::string ToString(){
            std::string live = "";
            if(isLive) live = "true";
            else live = "false";
            return "{\"title\", \"" + title + "\"}, {\"director\", \"" + director + "\"},{\"venue\", \"" + venue + "\"},{\"isLive\",\"" + live +"\"},";
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
                Proformance proformance;
                proformance.title = "TEST 1";
                proformance.director = "TEST 1";
                proformance.venue = "TEST 1";
                proformance.isLive = false;
                proformances.push_back(proformance);
                proformance.title = "TEST 2";
                proformance.director = "TEST 2";
                proformance.venue = "TEST 2";
                proformance.isLive = false;
                proformances.push_back(proformance);
                proformance.title = "TEST 3";
                proformance.director = "TEST 3";
                proformance.venue = "TEST 3";
                proformance.isLive = false;
                proformances.push_back(proformance);
            }
    };
}