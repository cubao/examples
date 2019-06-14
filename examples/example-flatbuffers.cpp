#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
    auto j = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
    std::cout << j.dump(4) << std::endl;
    return 0;
}
