#include "catch_headers.h"
#include "set.h"

#include <sstream>
#include <stdexcept>

TEST_CASE("Разбор нормализует вложенные множества и убирает повторы") {
    const Set value = Set::fromString(" {a, b, c, {a, b}, {}, {a, {c}}, {b, a, a}, a} ");
    const Set reordered = Set::fromString("{{{c},a}, {b,a}, c, b, a, { }}");
    CHECK(value.size() == 6);
    CHECK(value == reordered);
    CHECK(value["a"]);
    CHECK(value[" { b, a } "]);
    CHECK(value["{}"]);
    CHECK_FALSE(value["{a}"]);
    CHECK_FALSE(value["d"]);
}

TEST_CASE("Добавление вложенных множеств по значению и удаление") {
    Set value;
    CHECK(value.empty());
    Set nested = Set::fromString("{a, b}");
    value.add(nested);
    value.add("a");
    value.add("{b, a, a}");
    nested.add("c");
    CHECK(value.size() == 2);
    CHECK_FALSE(value["{a,b,c}"]);
    Set copy(value);
    copy.remove("{b,a}");
    copy.remove("missing");
    CHECK(copy == Set::fromString("{a}"));
    CHECK(copy != value);
    copy = value;
    CHECK(copy == value);
    value.add(value);
    CHECK(value["{a, {a, b}}"]);
}

TEST_CASE("Объединение, пересечение и разность принимают вложенные элементы") {
    const Set left = Set::fromString("{a, b, {a, b}}");
    const Set right = Set::fromString("{b, c, {b, a}}");
    const Set joined = Set::fromString("{a, b, c, {a, b}}");
    const Set common = Set::fromString("{b, {a, b}}");
    const Set difference = Set::fromString("{a}");
    CHECK(left + right == joined);
    CHECK(left * right == common);
    CHECK(left - right == difference);
    Set result = left;
    CHECK((result += right) == joined);
    CHECK((result += result) == joined);
    result = left;
    CHECK((result *= right) == common);
    result = left;
    CHECK((result -= right) == difference);
    result -= result;
    CHECK(result.empty());
}

TEST_CASE("Булеан включает пустое и исходное множества") {
    const Set empty;
    const Set emptyPower = empty.powerSet();
    CHECK(emptyPower == Set::fromString("{{}}"));
    const Set value = Set::fromString("{a, {b}}");
    const Set power = value.powerSet();
    CHECK(power.size() == 4);
    CHECK(power == Set::fromString("{{}, {a}, {{b}}, {a, {b}}}"));
}

TEST_CASE("Некорректный синтаксис и хвост отклоняются") {
    CHECK_THROWS_AS(Set::fromString(""), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("a"), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("{a"), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("{a b}"), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("{a,}"), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("{a,,b}"), std::invalid_argument);
    CHECK_THROWS_AS(Set::fromString("{} лишнее"), std::invalid_argument);
    Set value;
    CHECK_THROWS_AS(value.add("a b"), std::invalid_argument);
    CHECK_THROWS_AS(value.remove(" "), std::invalid_argument);
    CHECK(value.empty());
}

TEST_CASE("Поток читает по одному множеству и сохраняет объект при ошибке") {
    const Set original = Set::fromString("{a,{b},{}}");
    std::stringstream text;
    text << original << " {next}";
    CHECK(text.str() == "{a, {b}, {}} {next}");
    Set parsed;
    REQUIRE(text >> parsed);
    CHECK(parsed == original);
    REQUIRE(text >> parsed);
    CHECK(parsed == Set::fromString("{next}"));
    std::istringstream invalid("{ {a}");
    CHECK_FALSE(invalid >> parsed);
    CHECK(parsed == Set::fromString("{next}"));
}
