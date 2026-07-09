import json
def response(code, body):
    return {
        "statusCode": code,
        "headers": {
            "Content-Type": "application/json",
            "Access-Control-Allow-Origin": "*"
        },
        "body": json.dumps(body)
    }
def lambda_handler(event, context):
    authorizer_context = event.get("requestContext", {}).get("authorizer", {})
    user = authorizer_context.get("principalId", "unknown")
    return response(200, {
        "message": "You are authenticated!",
        "user":    user
    })