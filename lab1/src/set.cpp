#include "set.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
bool isAtomCharacter(int character) {
    return character != std::char_traits<char>::eof()
        && std::isspace(static_cast<unsigned char>(character)) == 0
        && character != '{' && character != '}' && character != ',';
}

void requireEnd(std::istream& input) {
    // peek() мог уже обнаружить EOF при чтении последнего символа слова.
    if (!input.eof()) {
        input >> std::ws;
    }
    if (!input.eof()) {
        throw std::invalid_argument("После объекта остались лишние символы.");
    }
}
}

Set Set::fromString(const std::string& text) {
    std::istringstream input(text);
    Set result = readSet(input);
    requireEnd(input);
    return result;
}

bool Set::empty() const { return elements_.empty(); }
std::size_t Set::size() const { return elements_.size(); }

void Set::add(const std::string& element) { elements_.insert(normalizeElement(element)); }
void Set::add(const Set& element) { elements_.insert(element.toString()); }
void Set::remove(const std::string& element) { elements_.erase(normalizeElement(element)); }

bool Set::operator[](const std::string& element) const {
    return elements_.count(normalizeElement(element)) != 0;
}

bool Set::operator==(const Set& other) const { return elements_ == other.elements_; }
bool Set::operator!=(const Set& other) const { return !(*this == other); }

Set Set::operator+(const Set& other) const {
    Set result = *this;
    result += other;
    return result;
}

Set Set::operator*(const Set& other) const {
    Set result;
    for (const auto& element : elements_) {
        if (other.elements_.count(element) != 0) {
            result.elements_.insert(element);
        }
    }
    return result;
}

Set Set::operator-(const Set& other) const {
    Set result;
    for (const auto& element : elements_) {
        if (other.elements_.count(element) == 0) {
            result.elements_.insert(element);
        }
    }
    return result;
}

Set& Set::operator+=(const Set& other) {
    elements_.insert(other.elements_.begin(), other.elements_.end());
    return *this;
}

Set& Set::operator*=(const Set& other) { return *this = *this * other; }
Set& Set::operator-=(const Set& other) { return *this = *this - other; }

Set Set::powerSet() const {
    std::vector<Set> subsets(1);  // Начинаем с единственного пустого подмножества.
    for (const auto& element : elements_) {
        const std::size_t previousSize = subsets.size();
        for (std::size_t index = 0; index < previousSize; ++index) {
            // Старые подмножества не содержат element, их новые копии содержат.
            Set extended = subsets[index];
            extended.elements_.insert(element);
            subsets.push_back(extended);
        }
    }
    Set result;
    for (const auto& subset : subsets) {
        result.add(subset);
    }
    return result;
}

std::string Set::toString() const {
    std::string result = "{";
    const char* separator = "";
    for (const auto& element : elements_) {
        result += separator;
        result += element;
        separator = ", ";
    }
    return result + "}";
}

Set Set::readSet(std::istream& input) {
    char delimiter = '\0';
    if (!(input >> delimiter) || delimiter != '{') {
        throw std::invalid_argument("Ожидалась открывающая скобка '{'.");
    }
    Set result;
    input >> std::ws;
    if (input.peek() == '}') {
        input.get();
        return result;
    }
    do {
        result.elements_.insert(readElement(input));
        if (!(input >> delimiter) || (delimiter != ',' && delimiter != '}')) {
            throw std::invalid_argument("Ожидалась запятая или закрывающая скобка '}'.");
        }
    } while (delimiter == ',');
    return result;
}

std::string Set::readElement(std::istream& input) {
    input >> std::ws;
    if (input.peek() == '{') {
        const Set nested = readSet(input);  // Для вложенного множества повторяем тот же разбор.
        return nested.toString();
    }
    std::string atom;
    while (isAtomCharacter(input.peek())) {
        atom += static_cast<char>(input.get());
    }
    if (atom.empty()) {
        throw std::invalid_argument("Ожидалось слово или вложенное множество.");
    }
    return atom;
}

std::string Set::normalizeElement(const std::string& text) {
    std::istringstream input(text);
    const std::string element = readElement(input);
    requireEnd(input);
    return element;
}

std::istream& operator>>(std::istream& input, Set& value) {
    try {
        value = Set::readSet(input);
    } catch (const std::invalid_argument&) {
        input.setstate(std::ios::failbit);
    }
    return input;
}

std::ostream& operator<<(std::ostream& output, const Set& value) {
    return output << value.toString();
}
