#include "catch_headers.h"
#include "vector3d.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

TEST_CASE("Вектор хранит обе точки, копия независима") {
    const Vector3D original({1, 2, 3}, {4, 6, 3});
    CHECK(original.start().x == 1);
    CHECK(original.start().y == 2);
    CHECK(original.start().z == 3);
    CHECK(original.end().x == 4);
    CHECK(original.end().y == 6);
    CHECK(original.end().z == 3);
    CHECK(original.length() == Catch::Approx(5.0));
    Vector3D copy(original);
    CHECK(copy == original);
    copy *= 2;
    CHECK(copy != original);
    copy = original;
    CHECK(copy == original);
}

TEST_CASE("Сумма и разность учитывают перемещения и сохраняют начало слева") {
    const Vector3D left({2, 3, 4}, {3, 5, 7});
    const Vector3D right({4, 7, 9}, {4, 8, 11});
    const Vector3D sum({2, 3, 4}, {3, 6, 9});
    const Vector3D difference({2, 3, 4}, {3, 4, 5});
    CHECK(left + right == sum);
    CHECK(left - right == difference);
    Vector3D result = left;
    CHECK((result += right) == sum);
    result = left;
    CHECK((result -= right) == difference);
    CHECK((result -= result) == Vector3D(left.start(), left.start()));
}

TEST_CASE("Векторное произведение меняет знак при перестановке сомножителей") {
    const Vector3D axisX({}, {1, 0, 0});
    const Vector3D axisY({}, {0, 1, 0});
    const Vector3D axisZ({}, {0, 0, 1});
    const Vector3D minusZ({}, {0, 0, -1});
    CHECK(axisX * axisY == axisZ);
    CHECK(axisY * axisX == minusZ);
    Vector3D result = axisX;
    CHECK((result *= axisY) == axisZ);
    CHECK((result *= result) == Vector3D());
}

TEST_CASE("Умножение и деление на число не сдвигают начало") {
    const Vector3D original({1, 2, 3}, {3, 0, 7});
    const Vector3D scaled({1, 2, 3}, {-3, 6, -5});
    const Vector3D divided({1, 2, 3}, {2, 1, 5});
    CHECK(original * -2 == scaled);
    CHECK(original / 2 == divided);
    Vector3D result = original;
    CHECK((result *= -2) == scaled);
    result = original;
    CHECK((result /= 2) == divided);
    CHECK_THROWS_AS(result /= 0, std::domain_error);
    CHECK(result == divided);
}

TEST_CASE("Косинус измеряет угол между ненулевыми перемещениями") {
    const Vector3D axisX({}, {1, 0, 0});
    const Vector3D axisY({}, {0, 1, 0});
    const Vector3D minusX({}, {-1, 0, 0});
    const Vector3D zero;
    CHECK(axisX.cosine(axisY) == Catch::Approx(0.0));
    CHECK(axisX.cosine(axisX) == Catch::Approx(1.0));
    CHECK(axisX.cosine(minusX) == Catch::Approx(-1.0));
    CHECK_THROWS_AS(axisX.cosine(zero), std::domain_error);
    CHECK_THROWS_AS(zero.cosine(axisX), std::domain_error);
}

TEST_CASE("Равенство точек отличается от сравнения длин") {
    const Vector3D axisX({}, {1, 0, 0});
    const Vector3D axisY({}, {0, 1, 0});
    const Vector3D translatedX({2, 0, 0}, {3, 0, 0});
    const Vector3D longer({}, {2, 0, 0});
    CHECK(axisX != axisY);
    CHECK(axisX != translatedX);
    CHECK(axisX <= axisY);
    CHECK(axisX >= axisY);
    CHECK_FALSE(axisX < axisY);
    CHECK_FALSE(axisX > axisY);
    CHECK(axisX < longer);
    CHECK(longer > axisX);
}

TEST_CASE("Потоки сохраняют точные координаты и не меняют вектор при ошибке") {
    const Vector3D original({0.12345678901234567, 2, 3}, {4, 5, 6});
    std::stringstream text;
    text << std::setprecision(3) << original;
    Vector3D parsed;
    REQUIRE(text >> parsed);
    CHECK(parsed == original);
    CHECK(text.precision() == 3);
    std::istringstream invalid("1 2 вместо_числа");
    CHECK_FALSE(invalid >> parsed);
    CHECK(parsed == original);
}
