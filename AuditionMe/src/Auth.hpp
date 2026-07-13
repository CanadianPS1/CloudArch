#include "../include/nlohmann/json.hpp"
namespace AuditionMe{   
    template <typename PrefixType>
    struct Authorization{
        nlohmann::json auth(nlohmann::json req){
            if(req["Bearer"] != "AuditionMe-2026.performer" && req["Bearer"] != "AuditionMe-2026.director") return {{"statusCode", 400}, {"Message", "Unauthenticated"}};
            return {{"policyDocument", {
                {"Version", "2026-7-13"},
                {"Statement", {
                    {"Action", "execute-api:Invoke"},
                    {"Effect", "Allow"}
                }}
            }}};
        }
    };
}