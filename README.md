# boost-helper-json
A simple wrapper around Boost.PropertyTree for quick &amp; dirty JSON access.

## 1. Introduction

If you're working with JSON on C++ and looking for a good library, I'd recommend [RapidJSON](http://rapidjson.org/) or [JSON for Modern C++](https://nlohmann.github.io/json/). They are fast and scalable, and provide loads of useful functionalities.

However, if you're just looking for a quick and dirty workaround, and happen to be using the Boost C++ libraries in your project, this may be an option for you.

Just include the header, and start hacking with JSON.

## 2. Example

```c++
#include "json.hpp"

#include <iostream>
#include <string>

int main()
{
    using namespace boost_helper;

    // Initialize JSON with a string.
    json response(R"({"err":0,"data":{"msg":"ok","names":["John","Sarah"]}})");

    // Changing a node value with a string.
    response["data"]["msg"] = "confirmed";

    // Check whether this node has a child with the specified key.
    if (response.has("err"))
    {
        // Changing a node value with another JSON node.
        response["err"] = json(R"({"code":0,"status":"ok"})");
    }

    // Access child node value of a JSON.
    std::cout << "Now message becomes \"" << response["data"]["msg"].val() << '"' << std::endl;

    // Iterate through a JSON node.
    for (const auto& name : response["data"]["names"])
    {
        std::cout << name.second.val() << " said hello!" << std::endl;
    }

    // Iterate througth a JSON node and change its children's values.
    for (auto name : response["data"]["names"])
    {
        name.second = "Nobody";
    }

    // Compare a JSON node with another node.
    if (response["data"]["names"] == json(R"(["Nobody","Nobody"])"))
    {
        // Add a child string to a JSON node without key.
        response["data"]["names"].add("Somebody");
    }

    // Add a child node to a JSON node without key.
    response["data"]["names"].add(json(R"(["Paul","Joshua"])"));

    // Print a JSON node.
    std::cout << response.to_string() << std::endl;
}
```

