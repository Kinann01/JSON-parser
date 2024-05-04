#include "../../include/JsonParser.h"

// Main methods supported
// 1) add a key : value to an object
// 2) add an element to an array
// 3) Modify an element in an array or a value of a key in an object.

// Testing functions
void companiesJSON() {

    JsonParser parser;
    std::string filename = "../testing/companies.json";

    // Read the JSON from a file and parse it
    JsonValue parsed_data = parser.readFromFile(filename);

    // Let's modify the "ceo" of the second company
    // Currently the CEO is null, we will modify it and name the ceo "John"
    // We have to define a path throw the JSON in order to know where we should modify
    // Path -> go to second element in the array -> find "ceo" and replace its value with "John"
    JsonValue::pathInJson path = {std::size_t(1), "ceo"};

    // False for isArray
    parsed_data.modifyJson(path, JsonValue(JsonValue::JsonString("John")), false);

    // Final result
    parser.printToFile("../testing/companies_updated.json", parsed_data);
}

void booksJSON(){
    // Create Object Parser
    JsonParser parser;

    // Provide the path to the file
    std::string filename = "../testing/books.json";

    // Read the JSON from a file and parse it
    JsonValue parsed_data = parser.readFromFile(filename);

    // Here we will modify the first ISBN for the third book in our JSON (books.json).
    // Currently, the first ISBN for the third book is 0316769487
    // We will describe a path through it and modify the element.
    // Then we will also add another ISBN for that book.
    JsonValue::pathInJson path = {"books", std::size_t(2), "isbn", std::size_t(0) };

    // True for isArray
    parsed_data.modifyJson(path, JsonValue(JsonValue::JsonString("95143194800")), true);

    // We will have to slightly modify to path in order to add another element to the array.
    // Our path has to end on that array and not in a particular element inside of it.

    JsonValue::pathInJson path2 = {"books", std::size_t(2), "isbn"};
    parsed_data.addToArray(path2, JsonValue(JsonValue::JsonString("55879103625")));

    // Final result
    parser.printToFile("../testing/books_updated.json", parsed_data);
}


void bookStoreJson(){

    JsonParser parser;
    std::string filename = "../testing/bookstore.json";
    JsonValue parsed_data = parser.readFromFile(filename);

    // Here we will add another object to the employees array that the bookstore employ.
    // Let's say the project is
    /*
     * {
            "hourlyRate": 18,
            "fullTime": true,
            "position": "Assistant",
            "name": "James Clone"
        },
     */

    // Our JsonValue holds an object which is std::unordered_map<std::string, JsonValue> {}
    // Similarly, as previously, lets find the path and insert that object
    // After we insert the object, we will start adding the key value pair one by one
    JsonValue::pathInJson path = {"bookstore", "employees"};
    parsed_data.addToArray(path, JsonValue(JsonValue::JsonObject{}));

    // Our path has to be modified in order to modify the particular project inside the projects array
    JsonValue::pathInJson path2 = {"bookstore", "employees", std::size_t(2)};
    parsed_data.addToObject(path2, "name", JsonValue(JsonValue::JsonString("James Clone")));
    parsed_data.addToObject(path2, "position", JsonValue(JsonValue::JsonString("Assistant")));
    parsed_data.addToObject(path2, "fullTime", JsonValue(JsonValue::JsonBool(true)));
    parsed_data.addToObject(path2, "hourlyRate", JsonValue(JsonValue::JsonNumber(18)));

    // Final result
    parser.printToFile("../testing/bookstore_updated.json", parsed_data);
}

int main() {

    try {
        companiesJSON();
    }
    catch (std::exception& e){
        std::cout<<e.what() << " in companiesJSON" <<std::endl;
    }

    try {
        booksJSON();
    }
    catch (std::exception& e){
        std::cout<<e.what() << " in booksJSON" <<std::endl;
    }

    try {
        bookStoreJson();
    }
    catch (std::exception& e){
        std::cout<<e.what() << " in bookStoreJson" <<std::endl;
    }

    return 0;
}