#ifndef LAB1_MENU_H
#define LAB1_MENU_H

#include <iosfwd>

/// Запускает главное меню.
/// Потоки передаются снаружи, чтобы CLI можно было тестировать.
void runMenu(std::istream& input, std::ostream& output);

#endif
