import json
from datetime import datetime, timezone
def lambda_handler(event, context):
    requiredFields = ["title", "director_name", "runtime_minutes", "release_year"]
    for field in requiredFields:
        if field not in event:
            return{
                "statusCode" : 400,
                "body" : json.dumbs({"message" : f"Missing required fields : {field}"})
            }
    runtimeMinutes = event["runtime_minutes"]
    if not isinstance(runtimeMinutes, int) or runtimeMinutes <= 0:
        return {
            "statusCode" : 400,
            "body" : json.dumps({"message" : "runtime minutes must be a positive integer"})
        }
    title = event["title"]
    directorName = event["director_name"]
    releaseYear = event["release_year"]
    current_year = datetime.now(timezone.utc).year
    decade = (releaseYear // 10) * 10
    is_classic = releaseYear < 1980
    runtime_hours  = round(runtimeMinutes / 60, 2)
    film_age_years = current_year - releaseYear
    return {
        "statusCode": 200,
        "body": json.dumps({
            "display":         f"{title} ({releaseYear})",
            "director":        directorName,
            "decade":          f"{decade}s",
            "is_classic":      is_classic,
            "runtime_hours":   runtime_hours,
            "film_age_years":  film_age_years,
            "generated_at":    datetime.now(timezone.utc).isoformat()
        })
    }
    #test
def run_local_tests():
    tests = [
        {
            "title" : "Death Stranding 2: On the Beach",
            "director_name" : "Hideo Kojima",
            "runtime_minutes" : 9999,
            "release_year" : 2025
        },{
            "title" : "Death Stranding 2: On the Beach",
            "director_name" : "Hideo Kojima",
            "runtime_minutes" : -1,
            "release_year" : 2025
        },{
            "title" : "The Grinch",
            "director_name" : "Hideo Kojima",
            "runtime_minutes" : 16,
            "release_year" : 90
        },{
            "title" : "Blue",
            "director_name" : "Hideo Kojima",
            "runtime_minutes" : 120,
            "release_year" : 1900
        }
    ]
    for event in tests:
        result = lambda_handler(event, None)
        print(f"Result: {result}")
if __name__ == "__main__":
    run_local_tests()
