#include "../include/nlohmann/json.hpp"
#include <string>
#include <string_view>
namespace AuditionMe{   
    template <typename PrefixType>
    struct Authorization{
        nlohmann::json auth(nlohmann::json req){
            const std::string token = req["authorizationToken"];
            const std::string methodArn = req["methodArn"];
            PrefixType prefix = "Basic ";
            if(!token.starts_with(prefix)) return {{"statusCode", 400}, {"message", "Unauthorized"}};
            //parse token
            return {};
        }
    };
}
/*
import base64
def lambda_handler(event, context):
    token = event.get("authorizationToken", "")
    method_arn = event.get("methodArn", "*")
    if not token or not token.startswith("Basic "):
        raise Exception("Unauthorized")
    try:
        encoded = token.split(" ", 1)[1]
        decoded = base64.b64decode(encoded).decode("utf-8")
        username, password = decoded.split(":", 1)
    except Exception:
        raise Exception("Unauthorized")
    if password != "cloudapi2026":
        return _policy(username, "Deny", method_arn)
    return _policy(username, "Allow", method_arn)
def _policy(principal_id, effect, method_arn):
    return {
        "principalId": principal_id,
        "policyDocument": {
            "Version": "2012-10-17",
            "Statement": [
                {
                    "Action":   "execute-api:Invoke",
                    "Effect":   effect,
                    "Resource": method_arn
                }
            ]
        }
    }

*/