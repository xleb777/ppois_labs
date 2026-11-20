#include "menu.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // В Windows консоль должна читать и показывать текст меню в UTF-8.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << std::boolalpha;
    runMenu(std::cin, std::cout);
}
