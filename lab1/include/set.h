#ifndef LAB1_SET_H
#define LAB1_SET_H

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>

/// Неупорядоченное множество слов и вложенных множеств без повторов.
class Set {
public:
    /// Создаёт пустое множество.
    Set() = default;
    /// Копирует содержимое независимо от оригинала.
    Set(const Set&) = default;
    /// Заменяет содержимое независимой копией.
    Set& operator=(const Set&) = default;
    /// Память автоматически освобождается контейнером std::set.
    ~Set() = default;

    /// Формирует множество из строки. При ошибке бросает std::invalid_argument.
    static Set fromString(const std::string& text);
    /// Проверяет, пусто ли множество.
    bool empty() const;
    /// Возвращает количество элементов верхнего уровня.
    std::size_t size() const;
    /// Добавляет слово или вложенное множество в текстовой записи.
    void add(const std::string& element);
    /// Добавляет другое множество как один элемент.
    void add(const Set& element);
    /// Удаляет элемент по текстовой записи, если он есть.
    void remove(const std::string& element);
    /// Проверяет принадлежность слова или вложенного множества.
    bool operator[](const std::string& element) const;
    /// Проверяет равенство по составу, независимо от порядка записи.
    bool operator==(const Set& other) const;
    /// Проверяет различие по составу.
    bool operator!=(const Set& other) const;
    /// Возвращает объединение.
    Set operator+(const Set& other) const;
    /// Возвращает пересечение.
    Set operator*(const Set& other) const;
    /// Возвращает разность: элементы слева, отсутствующие справа.
    Set operator-(const Set& other) const;
    /// Заменяет текущее множество объединением.
    Set& operator+=(const Set& other);
    /// Заменяет текущее множество пересечением.
    Set& operator*=(const Set& other);
    /// Заменяет текущее множество разностью.
    Set& operator-=(const Set& other);
    /// Возвращает все подмножества, включая пустое и исходное множество.
    Set powerSet() const;

private:
    // Элемент хранится как слово или отсортированная запись множества.
    std::set<std::string> elements_;

    std::string toString() const;
    static Set readSet(std::istream& input);
    static std::string readElement(std::istream& input);
    static std::string normalizeElement(const std::string& text);

    /// Читает одно множество в фигурных скобках, сохраняя старое при ошибке.
    friend std::istream& operator>>(std::istream& input, Set& value);
    /// Записывает множество в фигурных скобках.
    friend std::ostream& operator<<(std::ostream& output, const Set& value);
};

#endif
