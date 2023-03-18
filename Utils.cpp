#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include <Windows.h>

using action_handlers = std::map<std::string, std::function<void(void)>>;

void print_prelude()
{
    std::cout << "Prelude> ";
}

void print_code(std::string code)
{
    std::cout << "The character as a binary equals " << code << std::endl;
}

char get_code()
{
    char input;
    std::cin >> input;
    return input;
}

void run_action_handler(action_handlers* handlers, std::string key)
{
    auto handler = handlers->find(key);

    if (handler != handlers->end())
    {
        handler->second();
    }
}

std::string to_binary(int number)
{
    return std::bitset<8>(number).to_string();
}