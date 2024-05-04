#ifndef JSONVALUE_H
#define JSONVALUE_H

#include <variant>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

class JsonValue {
public:
	using JsonObject = std::unordered_map<std::string, JsonValue>;
	using JsonArray = std::vector<JsonValue>;
	using JsonString = std::string;
	using JsonNumber = double;
	using JsonBool = bool;
	using JsonNull = std::nullptr_t;
    using pathInJson = std::vector<std::variant<std::size_t, std::string>>;

private:
	using ValueType = std::variant<JsonObject, JsonArray, JsonString, JsonNumber, JsonBool, JsonNull>;
    ValueType value;

public:

	// Constructors for different JSON data types
	JsonValue() : value(JsonNull{}) {}
	explicit JsonValue(const JsonObject& obj) : value(obj) {}
	explicit JsonValue(const JsonArray& arr) : value(arr) {}
	explicit JsonValue(const JsonString& str) : value(str) {}
	explicit JsonValue(JsonNumber num) : value(num) {}
	explicit JsonValue(JsonBool b) : value(b) {}
	explicit JsonValue(JsonNull) : value(nullptr) {}

    // holding types
    bool isBool() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;
    bool isNull() const;

    // Getter methods
     const JsonObject& getAsObject() const;
     const JsonArray& getAsArray() const;
     const JsonString& getAsString() const;
     JsonNumber getAsNumber() const;
     JsonBool getAsBool() const;

    JsonObject& getAsObject();
    JsonArray& getAsArray();
    JsonString& getAsString();

    // Access Object && Access Array
    JsonValue& operator[](const JsonString& str);
    JsonValue& operator[](std::size_t idx);

    // Read
    const JsonValue& operator[](const JsonString& str) const;
    const JsonValue& operator[](std::size_t idx) const;

    // Modify elements in the data
    void modifyJson(const pathInJson&, JsonValue&&, bool);
    void addToObject( const pathInJson&, const std::string&, JsonValue&&);
    void addToArray( const pathInJson&, JsonValue&&);

    // Main Print
    void readablePrint(std::ostream& os, int indent = 0) const;

    // TODO
    // OTHER METHODS CAN BE ADDED
};

#endif
