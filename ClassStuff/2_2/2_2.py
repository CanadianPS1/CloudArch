import json
from datetime import datetime, timezone
def lambda_handler(event, context):
	body = json.dumps(event)
	if isinstance(body, str):
		try:
			body = json.loads(body)
		except json.JSONDecodeError:
			body = {}
	elif body is None :
		body = {}
	query = event.get("queryStringParameters") or {}
	headers = event.get("headers") or {}
	path = event.get("pathParameters") or {}
	return {
		"statusCode" : 200,
		"headers" : {
			"Content-Type" : "application/json"
		},
		"body" : json.dumps({
			"catagory" : path.get("catagory"),
			"verbose" : str(query.get("verbose", "false")).lower() == "true",
			"name" : body.get("name"),
			"value" : body.get("value"),
			"user_agent" : headers.get("user-agent"),
			"timestamp" : datetime.now(timezone.utc).isoformat()
		})
	}