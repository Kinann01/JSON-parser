#ifndef JSONPARSER_JSONPARSER_H
#define JSONPARSER_JSONPARSER_H

#ifndef JSONVALUE_
#include "JsonValue.h"
#endif

#ifndef VECTOR_
#include <vector>
#endif

#ifndef IOSTREAM_
#include <iostream>
#endif

#ifndef EXCEPTION_
#include <stdexcept>
#endif

#ifndef FSTREAM_
#include <fstream>
#endif


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


    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    // Function to read JSON data from a file and parse it
    JsonValue readFromFile(const std::string&);

    // Function to write a Json format to a file
    void printToFile(const std::string&, const JsonValue&);

private:

    // TOKENIZER TOOLS

    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    static std::vector<Token> tokenizeJSONfile(std::ifstream& stream){
        std::vector<Token> tokens;
        std::string buffer;

        // Read file character by character
        char c;
        while (stream.get(c)) {
            // get rid of white spaces
            if (isspace(c)){
                continue;
            }

            // Switch for types of tokens

            switch (c) {
                case '{':
                    tokens.push_back({TokenType::ObjectOpen, "{"});
                    break;
                case '}':
                    tokens.push_back({TokenType::ObjectClose, "}"});
                    break;
                case '[':
                    tokens.push_back({TokenType::ArrayOpen, "["});
                    break;
                case ']':
                    tokens.push_back({TokenType::ArrayClose, "]"});
                    break;
                case ',':
                    tokens.push_back({TokenType::Comma, ","});
                    break;
                case ':':
                    tokens.push_back({TokenType::Colon, ":"});
                    break;
                case '"':
                    buffer.clear();
                    while (stream.get(c) && c != '"'){
                        if (c == '\\' && stream.peek() != EOF) {
                            stream.get(c); // Get next character
                            switch (c) {
                                case '\\': buffer.push_back('\\'); break;
                                case '"': buffer.push_back('"'); break;
                                case 'n': buffer.push_back('\n'); break;
                                case 't': buffer.push_back('\t'); break;
                                default: buffer.push_back(c); break;
                            }
                        } else {
                            buffer.push_back(c);
                        }
                    }
                    tokens.push_back({TokenType::String, buffer});
                    break;

                default:
                    buffer.clear();
                    while (!isspace(c) && c != ',' && c != ':' && c != '{' && c != '}' && c != '[' && c != ']' && stream.peek() != EOF) {
                        buffer.push_back(c);
                        stream.get(c);
                    }
                    // Step back to process the delimiter in the next iteration
                    stream.unget();

                    if (buffer == "true") {
                        tokens.push_back({TokenType::True, buffer});
                    } else if (buffer == "false") {
                        tokens.push_back({TokenType::False, buffer});
                    } else if (buffer == "null") {
                        tokens.push_back({TokenType::Null, buffer});
                    } else {
                        tokens.push_back({TokenType::Number, buffer});
                    }
                    break;
            }
        }

        return tokens;
    }


    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------

    // Main transition Function
    JsonValue parseValue(const std::vector<Token>& tokens, size_t& index);

    // Helper transition function to parse an Object inside an object
    JsonValue::JsonObject parseObject(const std::vector<Token>& tokens, size_t& index);

    // Helper transition function to parse an array inside an object
    JsonValue::JsonArray parseArray(const std::vector<Token>& tokens, size_t& index);

};

#endif //JSONPARSER_JSONPARSER_H