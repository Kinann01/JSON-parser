# JSON Parser API

## Description

The JSON Parser API is a C++17 project that allows users to read, parse, modify, and write JSON data from and to files. The key functionalities of the API include:

- Reading JSON data from a file
- Parsing the data into a usable format
- Modifying the data, either by adding an element to an array or an object or by modifying an existing element
- Writing the modified data back to a file

This API is particularly useful for users who need to manipulate JSON files programmatically, as it provides a simple, straightforward interface for doing so.

## Installation

To install the JSON Parser API, you will need a C++17 (or later) compiler. 

### How to compile?

    After cloning the repository, navigate to the root of the reposity and you can build the project as follows:

        Option 1) The provided CMakeLists.txt file can be used to build the project
        Option 2) g++ -std=c++17 src/main/main.cpp src/JsonValue.cpp src/JsonParser.cpp

## Usage

Here are the main that are supported:

- `void JsonValue::modifyJson(const pathInJson & path, JsonValue&& newValue, bool isArray = false)`: This function is used to modify an existing value in your JSON data. The `path` parameter is a vector of variants (either a string or a size_t), which describes the path through the JSON data to the element you want to modify. `newValue` is the new value that you want to set. The `isArray` flag should be set to true if the parent of the element you're modifying is an array.


- `void JsonValue::addToObject(const pathInJson &path, const std::string& newKey, JsonValue&& newValue)`: This function is used to add a new key-value pair to an existing JSON object. The `path` describes the path through the JSON data to the object you want to modify. `newKey` is the key of the new element, and `newValue` is its value.


- `void JsonValue::addToArray(const pathInJson& path, JsonValue&& newValue)`: This function is used to add a new element to an existing JSON array. The `path` describes the path through the JSON data to the array you want to modify, and `newValue` is the value of the new element.

The `path` parameter in these methods is crucial as it guides the method to the exact location in the JSON structure where the operation (modification or addition) is to be performed. It can be a sequence of keys (for objects) and indices (for arrays), navigated in the order of the structure's depth. If the provided path is incorrect or doesn't lead to an expected type (like an object or an array), a runtime error will be thrown. Therefore, it's essential to ensure the correctness of the provided path.

Here's a basic usage example:

```cpp
// Create a JsonParser instance
JsonParser parser;

// Provide the path to the JSON file
std::string filename = "../testing/books.json";

// Read the JSON from the file and parse it
JsonValue parsed_data = parser.readFromFile(filename);

// Specify the path to the element to be modified in the JSON
JsonValue::pathInJson path = {"books", std::size_t(2), "isbn", std::size_t(0) };

// Modify the specified element
parsed_data.modifyJson(path, JsonValue(JsonValue::JsonString("95143194800")), true);

// Print the modified JSON data to a new file
parser.printToFile("../testing/books_updated.json", parsed_data);
```

## Testing

- Access the `src/main/main.cpp` file. It contains 3 examples that test the correctness. All the necessary comments for elaboration are provided.