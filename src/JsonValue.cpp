#ifndef JSONVALUE_
#include "JsonValue.h"
#endif


bool JsonValue::isBool() const {
    return std::holds_alternative<JsonBool>(value);
}
bool JsonValue::isNumber() const {
    return std::holds_alternative<JsonNumber>(value);
}
bool JsonValue::isString()  const {
    return std::holds_alternative<JsonString>(value);
}

bool JsonValue::isArray() const {
    return std::holds_alternative<JsonArray>(value);
}

bool JsonValue::isObject() const {
    return std::holds_alternative<JsonObject>(value);
}

bool JsonValue::isNull() const {
    return std::holds_alternative<JsonNull>(value);
}

// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------

const JsonValue::JsonObject& JsonValue::getAsObject() const {
    if (isObject()){
        return std::get<JsonObject>(value);
    }

    else{
        throw std::runtime_error("Bad Access, not an object");
    }
}

const JsonValue::JsonArray& JsonValue::getAsArray() const {
    if (isArray()){
        return std::get<JsonArray>(value);
    }

    else{
        throw std::runtime_error("Bad Access, not an array");
    }
}

const JsonValue::JsonString& JsonValue::getAsString() const {
    if (isString()){
        return std::get<JsonString>(value);
    }

    else{
        throw std::runtime_error("Bad Access, not a string");
    }
}

JsonValue::JsonNumber JsonValue::getAsNumber() const {
    if (isNumber()){
        return std::get<JsonNumber>(value);
    }

    else{
        throw std::runtime_error("Bad Access, not a number");
    }
}

JsonValue::JsonBool JsonValue::getAsBool() const {
    if (isBool()){
        return std::get<JsonBool>(value);
    }

    else{
        throw std::runtime_error("Bad Access, not a boolean");
    }
}

JsonValue::JsonObject& JsonValue::getAsObject() {
    if (!isObject()){
        throw std::runtime_error("Bad Access, not an object");
    }

    return std::get<JsonObject>(value);
}

JsonValue::JsonArray& JsonValue::getAsArray() {
    if (!isArray()){
        throw std::runtime_error("Bad Access, not an array");
    }

    return std::get<JsonArray>(value);
}

JsonValue::JsonString& JsonValue::getAsString() {
    if (!isString()){
        throw std::runtime_error("Bad Access, not a string");
    }

    return std::get<JsonString>(value);
}


// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------


JsonValue& JsonValue::operator[](const JsonString& str) {
    if (!isObject()){
        throw std::runtime_error("JsonValue is not an Object");
    }

    return std::get<JsonObject>(value)[str];
}

JsonValue& JsonValue::operator[](std::size_t idx) {
    if (!isArray()){
        throw std::runtime_error("JsonValue is not an Array");
    }

    return std::get<JsonArray>(value)[idx];
}

const JsonValue& JsonValue::operator[](const JsonString & str) const {
    if (!isObject()){
        throw std::runtime_error("JsonValue is not an Object");
    }

    auto &getter = getAsObject();
    auto it = getter.find(str);
    if (it == getter.end()){
        throw std::runtime_error("Not able to find value to the given key");
    }

    return it->second;
}


const JsonValue& JsonValue::operator[](std::size_t idx) const{

    if (!isArray()){
        throw std::runtime_error("JsonValue is not an Array");
    }

    auto &getter = getAsArray();

    if ( idx > getter.size()){
        throw std::runtime_error("Index out of range, unable to access array");
    }

    return getter[idx];

}



// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------


void JsonValue::readablePrint(std::ostream &os, int indent) const {
    {
        const std::string indentStr(indent * 2, ' ');

        if (isObject()) {
            const JsonObject& obj = getAsObject();
            os << "{\n";
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                os << indentStr << "  \"" << it->first << "\": ";
                it->second.readablePrint(os, indent + 1);
                if (std::next(it) != obj.end()) {
                    os << ",";
                }
                os << "\n";
            }
            os << indentStr << "}";
        } else if (isArray()) {

            const JsonArray& arr = getAsArray();
            os << "[\n";

            for (auto it = arr.begin(); it != arr.end(); ++it) {
                os << indentStr << "  ";
                it->readablePrint(os, indent + 1);
                if (std::next(it) != arr.end()) {
                    os << ",";
                }
                os << "\n";
            }
            os << indentStr << "]";
        } else if (isString()) {

            os << "\"" << getAsString()<< "\"";
        } else if (isNumber()) {
            os << getAsNumber();

        } else if (isBool()) {
            os << (getAsBool() ? "true" : "false");

        } else if (isNull()) {
            os << "null";
        }
        else{
            os << "Nothing to print";
        }
    }
}

// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------

void JsonValue::modifyJson(const pathInJson & path, JsonValue&& newValue, bool isArray = false) {

    JsonValue* current = this;
    JsonValue* temp = this;

    const auto & itr_second_last_element = std::prev(path.end(), 2);

    for (const auto &key : path) {
        std::visit([&](auto&& arg) {

            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::size_t>){

                if (current->isArray()){
                    // access array
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected array but got different type");
                }
            }

            else if constexpr (std::is_same_v<T, std::string>) {

                if (current->isObject()){
                    // access object
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected object but got different type");
                }
            }

            if (key == *itr_second_last_element){

                temp = current;
            }

        }, key);
    }


    if (isArray){
        if (temp->isArray()){
            *current = std::move(newValue);
        }
        else{
            throw std::runtime_error("Expected Array but got different type");
        }
    }

    else{
        if (temp->isObject()){
            *current = std::move(newValue);
        }
    }
}

// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------


void JsonValue::addToObject(const pathInJson &path, const std::string& newKey, JsonValue&& newValue) {


    JsonValue* current = this;


    for (const auto &p : path){

        std::visit([&](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::size_t>){
                if (current->isArray()){
                    // access array
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected array but got different type");
                }
            }

            else if constexpr (std::is_same_v<T, std::string>) {

                if (current->isObject()){
                    // access object
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected object but got different type");
                }
            }
        }, p);
    }


    // Ensure the current element is an object.

    if (!current->isObject()) {
        throw std::runtime_error("Path does not lead to an object");
    }

    (*current)[newKey] = std::move(newValue);
}


// pathInJson = std::vector<std::variant<std::size_t, std::string>>
// JsonValue = variant<Number, bool, null, array, object, string>
void JsonValue::addToArray(const pathInJson& path, JsonValue&& newValue) {

    JsonValue* current = this;

    // Traverse the path to find the parent of the element to be added.
    for (const auto &key : path) {
        std::visit([&](auto&& arg) {

            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::size_t>) {
                if (current->isArray()) {
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected array but got different type");
                }

            }

            else if constexpr (std::is_same_v<T, std::string>) {

                if (current->isObject()) {
                    current = &(*current)[arg];
                }
                else {
                    throw std::runtime_error("Expected object but got different type");
                }
            }
        }, key);
    }

    // Ensure the current element is an array.
    if (!current->isArray()) {
        throw std::runtime_error("Expected array but got different type");
    }

    current->getAsArray().push_back(std::move(newValue));
}

