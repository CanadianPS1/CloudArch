import json
from datetime import datetime, timezone
def lambda_handler(event, context):
    inputText = event.get("imput_text", "")
    return {
        "echo" : inputText,
        "reversed" : inputText[::-1],
        "length" : len(inputText),
        "timestamp" : datetime.now(timezone.utc).isoformat()
    }
def run_local_tests():
    print("Hello World")
    test = [
        {"inputText" : "Hello AWS!"},
        {"inputText" : "GoodBye AWS!"},
        {"inputText" : "x"},
        {}
    ]
    for event in test:
        result = lambda_handler(event, None)
        print(f"Input :  {event}")
        print(f"Result :  {result}")
        print()
if __name__ == "__main__":
    run_local_tests()
