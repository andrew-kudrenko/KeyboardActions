#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include <Windows.h>

using action_handlers = std::map<std::string, std::function<void(void)>>;

using key_descriptions = std::map<std::string, std::string>;

std::string to_binary(int number)
{
    return std::bitset<8>(number).to_string();
}

void print_prelude()
{
    std::cout << "Prelude> ";
}

void print_code(std::string as_binary, char raw)
{
    std::cout << "The character \"" << raw << "\" as a binary equals " << as_binary << std::endl;
}

void print_guide(key_descriptions* descriptions, char exit_on)
{
    std::cout << "The program may be terminated by entering \"" << exit_on << "\" or \"Ctrl^C\"\n\n";
    std::cout << "Available commands:" << std::endl << std::endl;

    for (const auto& pair : *descriptions) 
    {
        std::cout << "\t~ " << pair.first << " - " << pair.second << std::endl;
    }

    std::cout << std::endl;
}

char get_code()
{
    char input;

    std::cin >> input;
    std::cin.ignore(1024, '\n');
    
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

void open_explorer()
{
    std::cout << "Opening explorer..." << std::endl;
    ShellExecute(NULL, NULL, L"explorer", NULL, NULL, SW_SHOWNORMAL);
}

void close_explorer()
{
    HWND hwnd = FindWindow(L"CabinetWClass", NULL);

    if (hwnd != NULL) 
    {
        std::cout << "Closing explorer..." << std::endl;
        SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
    }
    else
    {
        std::cout << "Explorer isn't opened" << std::endl;
    }
}

void open_folder()
{
    std::cout << "Opening a directory with path> ";

    TCHAR path[MAX_PATH];
    std::wcin >> path;

    ShellExecute(NULL, L"open", path, NULL, NULL, SW_SHOW);
}

void create_file()
{
    char path[MAX_PATH];

    std::cout << "Enter a path to create a file> ";
    std::cin >> path;

    std::ofstream fout;
    
    try
    {
        std::cout << "Creating a file by path " << path << std::endl;
        fout.open(path, std::ios_base::out);
    }
    catch (const std::exception) {
        std::cout << "An error ocurred during creating the file" << std::endl;
    }

    fout.close();
}

void remove_file()
{
    char path[MAX_PATH];

    std::cout << "Enter a path to remove a file> ";
    std::cin >> path;

    remove(path);
}

void poll_user(action_handlers* handlers, std::string& code)
{
    print_prelude();

    char raw_code = get_code();
    code = to_binary(raw_code);

    print_code(code, raw_code);
    run_action_handler(handlers, code);

    std::cout << std::endl;
}

int main()
{
    key_descriptions descriptions = std::map<std::string, std::string>({
        {"K", "Open explorer"},
        {"H", "Close explorer"},
        {"L", "Open folder"},
        {"I", "Remove file"},
        {"u", "Create file"},
    });

    char exit_on_raw = 'y';
    std::string exit_on = to_binary(exit_on_raw);

    action_handlers handlers = std::map<std::string, std::function<void (void)>>({
        {to_binary('K'), open_explorer},
        {to_binary('H'), close_explorer},
        {to_binary('L'), open_folder},
        {to_binary('I'), remove_file},
        {to_binary('u'), create_file},
    });
    std::string code;

    print_guide(&descriptions, exit_on_raw);

    do
    {
        poll_user(&handlers, code);
    } while (code != exit_on);

    return 0;
}
