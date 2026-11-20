#include "menu.h"
#include "set.h"
#include "vector3d.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace {
constexpr std::size_t maxPowerSetInputSize = 12;
constexpr int backCommand = 0;

enum class VectorCommand {
    enterLeft = 1, enterRight, show, add, subtract, cross, scale, divide, cosine, compare
};

enum class SetCommand {
    enterLeft = 1, enterRight, show, addElement, removeElement, contains, join, intersect, subtract, power, compare
};

constexpr const char* vectorMenu =
    "\nВекторы: 1. Ввести A  2. Ввести B  3. Координаты и длины\n"
    "4. A+B  5. A-B  6. A*B  7. A*число  8. A/число  9. Косинус  10. Сравнение  0. Назад\n> ";
constexpr const char* setMenu =
    "\nМножества: 1. Ввести A  2. Ввести B  3. Вывести A и B\n"
    "4. Добавить в A  5. Удалить из A  6. Проверить принадлежность A\n"
    "7. A+B  8. A*B  9. A-B  10. Булеан A  11. Сравнение  0. Назад\n> ";

std::string ask(std::istream& input, std::ostream& output, const char* prompt) {
    output << prompt;
    std::string line;
    std::getline(input, line);
    return line;
}

template <typename Value>
Value readValue(std::istream& input, std::ostream& output, const char* prompt) {
    const std::string text = ask(input, output, prompt);
    std::istringstream line(text);
    Value value{};
    if (!(line >> value)) {
        throw std::invalid_argument("Неверный формат ввода.");
    }
    line >> std::ws;
    if (!line.eof()) {
        throw std::invalid_argument("После значения остались лишние символы.");
    }
    return value;
}

void showVectorComparison(const Vector3D& left, const Vector3D& right, std::ostream& output) {
    output << "По координатам: A == B: " << (left == right) << ", A != B: " << (left != right) << '\n'
           << "По длине: A < B: " << (left < right) << ", A <= B: " << (left <= right)
           << ", A > B: " << (left > right) << ", A >= B: " << (left >= right) << '\n';
}

void showScaledVector(VectorCommand command, const Vector3D& value, std::istream& input, std::ostream& output) {
    if (command == VectorCommand::scale) {
        const double factor = readValue<double>(input, output, "Множитель: ");
        output << value * factor << '\n';
        return;
    }
    const double divisor = readValue<double>(input, output, "Делитель: ");
    output << value / divisor << '\n';
}

void vectorCommand(int command, Vector3D& left, Vector3D& right, std::istream& input, std::ostream& output) {
    constexpr const char* vectorPrompt = "Введите x1 y1 z1 x2 y2 z2: ";
    const auto action = static_cast<VectorCommand>(command);
    switch (action) {
        case VectorCommand::enterLeft: left = readValue<Vector3D>(input, output, vectorPrompt); break;
        case VectorCommand::enterRight: right = readValue<Vector3D>(input, output, vectorPrompt); break;
        case VectorCommand::show:
            output << "A: " << left << "; длина: " << left.length() << '\n'
                   << "B: " << right << "; длина: " << right.length() << '\n';
            break;
        case VectorCommand::add: output << "A+B = " << (left + right) << '\n'; break;
        case VectorCommand::subtract: output << "A-B = " << (left - right) << '\n'; break;
        case VectorCommand::cross: output << "A*B = " << (left * right) << '\n'; break;
        case VectorCommand::scale:
        case VectorCommand::divide: showScaledVector(action, left, input, output); break;
        case VectorCommand::cosine: {
            const double cosine = left.cosine(right);
            output << "Косинус: " << cosine << '\n';
            break;
        }
        case VectorCommand::compare: showVectorComparison(left, right, output); break;
        default: output << "Нет такого пункта.\n"; break;
    }
}

void showSet(const char* name, const Set& value, std::ostream& output) {
    output << name << ": " << value << "; мощность: " << value.size()
           << "; пустое: " << value.empty() << '\n';
}

void editSet(SetCommand command, Set& value, std::istream& input, std::ostream& output) {
    const std::string element = ask(input, output, "Введите слово или вложенное множество: ");
    if (command == SetCommand::addElement) {
        value.add(element);
    } else if (command == SetCommand::removeElement) {
        value.remove(element);
    } else {
        output << "Элемент принадлежит A: " << value[element] << '\n';
        return;
    }
    showSet("A", value, output);
}

void showPowerSet(const Set& value, std::ostream& output) {
    // Ограничение только для меню: число подмножеств растёт как 2 в степени n.
    if (value.size() > maxPowerSetInputSize) {
        output << "Для показа булеана нужно не больше " << maxPowerSetInputSize << " элементов.\n";
        return;
    }
    showSet("Булеан A", value.powerSet(), output);
}

void setCommand(int command, Set& left, Set& right, std::istream& input, std::ostream& output) {
    constexpr const char* setPrompt = "Введите множество, например {a, {b}, {}}: ";
    const auto action = static_cast<SetCommand>(command);
    switch (action) {
        case SetCommand::enterLeft: left = readValue<Set>(input, output, setPrompt); break;
        case SetCommand::enterRight: right = readValue<Set>(input, output, setPrompt); break;
        case SetCommand::show: showSet("A", left, output); showSet("B", right, output); break;
        case SetCommand::addElement:
        case SetCommand::removeElement:
        case SetCommand::contains: editSet(action, left, input, output); break;
        case SetCommand::join: showSet("A+B", left + right, output); break;
        case SetCommand::intersect: showSet("A*B", left * right, output); break;
        case SetCommand::subtract: showSet("A-B", left - right, output); break;
        case SetCommand::power: showPowerSet(left, output); break;
        case SetCommand::compare:
            output << "A == B: " << (left == right) << ", A != B: " << (left != right) << '\n';
            break;
        default: output << "Нет такого пункта.\n"; break;
    }
}

// Подменю читают номер и обрабатывают ошибки одинаково.
// Отличаются тип объектов и функция, которая выполняет выбранную команду.
template <typename Value>
void runSubmenu(std::istream& input, std::ostream& output, const char* prompt,
                void (*execute)(int, Value&, Value&, std::istream&, std::ostream&)) {
    Value left;
    Value right;
    while (true) {
        try {
            const int command = readValue<int>(input, output, prompt);
            if (command == backCommand) {
                return;
            }
            execute(command, left, right, input, output);
        } catch (const std::logic_error& error) {
            if (!input) {
                return;
            }
            output << "Ошибка: " << error.what() << '\n';
        }
    }
}
}

void runMenu(std::istream& input, std::ostream& output) {
    while (input) {
        const std::string command = ask(input, output, "\n1. Векторы  2. Множества  0. Выход\n> ");
        if (!input || command == "0") {
            return;
        }
        if (command == "1") {
            runSubmenu(input, output, vectorMenu, vectorCommand);
        } else if (command == "2") {
            runSubmenu(input, output, setMenu, setCommand);
        } else {
            output << "Нет такого пункта.\n";
        }
    }
}
