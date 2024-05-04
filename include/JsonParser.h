#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JsonValue.h"
#include <vector>
#include <iostream>
#include <stdexcept>


class JsonParser {

private:

    enum class TokenType {
        ObjectOpen, // {
        ObjectClose, // }
        ArrayOpen, // [
        ArrayClose, // ]
        String, // "Hello"
        Number, // 3.5, 3, -3, 0, e, pi
        True, // true
        False, // false
        Null, // null
        Colon, // :
        Comma // ,
    };

    struct Token {
        TokenType type;
        std::string value;
    };

public:

    // Main parsing function - Contains the starting state and the accepting state.
    JsonValue parse(const std::vector<Token>&);

    // Function to read JSON data from a file and parse it
    JsonValue readFromFile(const std::string&);

    // Function to write a Json format to a file
    void printToFile(const std::string&, const JsonValue&);

private:

    // Tokenize the JSON
    static std::vector<Token> tokenizeJSONfile(std::ifstream&);

    // Main transition Function
    JsonValue parseValue(const std::vector<Token>&, size_t&);

    // Helper transition function to parse an Object inside an object
    JsonValue::JsonObject parseObject(const std::vector<Token>&, size_t&);

    // Helper transition function to parse an array inside an object
    JsonValue::JsonArray parseArray(const std::vector<Token>&, size_t&);

};

#endif