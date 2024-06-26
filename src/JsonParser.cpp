#include "../include/JsonParser.h"
#include <fstream>
#include <iostream>

JsonValue JsonParser::parse(const std::vector<Token> & tokens){

    size_t index = 0;
    JsonValue result = parseValue(tokens, index);

    if (index != tokens.size()) {
        throw std::runtime_error("Unexpected tokens after the end of the JSON input");
    }
    return result;
}



JsonValue JsonParser::readFromFile(const std::string & filename) {
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Unable to open file");
    }

    std::vector<Token> tokens = tokenizeJSONfile(file);
    file.close();
    return parse(tokens);
}



void JsonParser::printToFile(const std::string & newFileName, const JsonValue & result) {
    std::ofstream file(newFileName);

    if (!file.is_open()){
        throw std::runtime_error("Unable to open file");
    }

    result.readablePrint(file);
    file.close();
}

JsonValue JsonParser::parseValue(const std::vector<Token> &tokens, size_t &index){
    {

        if (index >= tokens.size()) {
            throw std::runtime_error("Unexpected end of input");
        }

        // First index is the beginning of the jsonFile
        const Token& token = tokens[index++];

        // Based on the type, the automaton reads the beginning of the json file, we go to the next state accordingly.
        switch (token.type) {
            case TokenType::ObjectOpen:
                return JsonValue(parseObject(tokens, index));

            case TokenType::ArrayOpen:
                return JsonValue(parseArray(tokens, index));

            case TokenType::String:
                return JsonValue(JsonValue::JsonString(token.value));

            case TokenType::Number: {
                try {
                    double num = std::stod(token.value);
                    return JsonValue(JsonValue::JsonNumber(num));
                } catch (const std::exception& e) {
                    throw std::runtime_error("Invalid number format: " + token.value);
                }
            }

            case TokenType::True:
                return JsonValue(JsonValue::JsonBool(true));

            case TokenType::False:
                return JsonValue(JsonValue::JsonBool(false));

            case TokenType::Null:
                return JsonValue(JsonValue::JsonNull{});
            default:
                throw std::runtime_error("Unexpected token type");
        }
    }
}

JsonValue::JsonArray JsonParser::parseArray(const std::vector<Token> &tokens, size_t &index) {
    JsonValue::JsonArray arr;

    while (index < tokens.size() && tokens[index].type != TokenType::ArrayClose) {
        arr.push_back(parseValue(tokens, index));

        if (index < tokens.size() && tokens[index].type == TokenType::Comma) {
            index++;
        }
    }

    if (index >= tokens.size() || tokens[index].type != TokenType::ArrayClose) {
        throw std::runtime_error("Expected a closing bracket for the JSON array");
    }

    index++; // Skip the closing bracket
    return arr;
}

JsonValue::JsonObject JsonParser::parseObject(const std::vector<Token> &tokens, size_t &index) {
    JsonValue::JsonObject obj;

    while (index < tokens.size() && tokens[index].type != TokenType::ObjectClose) {
        if (tokens[index].type != TokenType::String) {
            throw std::runtime_error("Expected a string key in the JSON object");
        }

        std::string key = tokens[index++].value;

        if (index >= tokens.size() || tokens[index].type != TokenType::Colon) {
            throw std::runtime_error("Expected a colon after the JSON object key");
        }

        index++; // Skip the colon
        JsonValue value = parseValue(tokens, index);

        if (index < tokens.size() && tokens[index].type == TokenType::Comma) {
            index++; // Skip the comma
        }

        obj[std::move(key)] = std::move(value);
    }

    if (index >= tokens.size() || tokens[index].type != TokenType::ObjectClose) {
        throw std::runtime_error("Expected a closing brace for the JSON object");
    }

    index++; // Skip the closing brace
    return obj;
}

std::vector<JsonParser::Token> JsonParser::tokenizeJSONfile(std::ifstream & stream) {
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
                while (!isspace(c) && c != ',' && c != ':' && c != '{' && c != '}'
                       && c != '[' && c != ']' && stream.peek() != EOF) {

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