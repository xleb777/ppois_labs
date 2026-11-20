#include "catch_headers.h"
#include "menu.h"

#include <iomanip>
#include <sstream>
#include <string>

namespace {
std::string menuResult(const std::string& commands) {
    std::istringstream input(commands);
    std::ostringstream output;
    output << std::boolalpha;
    runMenu(input, output);
    return output.str();
}
}

TEST_CASE("Меню векторов выполняет операции и возвращается в главное меню") {
    const std::string output = menuResult(
        "нет\n1\n1\n0 0 0 2 0 0\n2\n0 0 0 0 1 0\n3\n4\n5\n6\n7\n2\n8\n2\n9\n10\n99\n0\n0\n");
    CHECK(output.find("длина: 2") != std::string::npos);
    CHECK(output.find("A+B = 0 0 0 2 1 0") != std::string::npos);
    CHECK(output.find("A-B = 0 0 0 2 -1 0") != std::string::npos);
    CHECK(output.find("A*B = 0 0 0 0 0 2") != std::string::npos);
    CHECK(output.find("Множитель: 0 0 0 4 0 0") != std::string::npos);
    CHECK(output.find("Делитель: 0 0 0 1 0 0") != std::string::npos);
    CHECK(output.find("Косинус: 0") != std::string::npos);
    CHECK(output.find("A > B: true") != std::string::npos);
    CHECK(output.find("Нет такого пункта") != std::string::npos);
}

TEST_CASE("Меню множеств проверяет принадлежность и строит булеан") {
    const std::string output = menuResult("2\n1\n{a,{b}}\n2\n{a,c}\n3\n4\n{}\n6\n{}\n5\n{}\n7\n8\n9\n10\n11\n99\n0\n0\n");
    CHECK(output.find("B: {a, c}; мощность: 2; пустое: false") != std::string::npos);
    CHECK(output.find("Элемент принадлежит A: true") != std::string::npos);
    CHECK(output.find("A+B: {a, c, {b}}") != std::string::npos);
    CHECK(output.find("A*B: {a}") != std::string::npos);
    CHECK(output.find("A-B: {{b}}") != std::string::npos);
    CHECK(output.find("Булеан A:") != std::string::npos);
    CHECK(output.find("мощность: 4") != std::string::npos);
    CHECK(output.find("A != B: true") != std::string::npos);
}

TEST_CASE("Меню обрабатывает ошибки и завершение входного потока") {
    const std::string vectors = menuResult("1\n1\n1 2 3 4 5 6 хвост\n8\n0\n9\n\n0\n0\n");
    CHECK(vectors.find("остались лишние символы") != std::string::npos);
    CHECK(vectors.find("Делить вектор на ноль нельзя") != std::string::npos);
    CHECK(vectors.find("Косинус для нулевого вектора не определён") != std::string::npos);
    CHECK(vectors.find("Неверный формат ввода") != std::string::npos);
    const std::string sets = menuResult("2\n1\n{a,b,c,d,e,f,g,h,i,j,k,l,m}\n10\n");
    CHECK(sets.find("не больше 12 элементов") != std::string::npos);
    CHECK(sets.find("Ошибка:") == std::string::npos);
}
