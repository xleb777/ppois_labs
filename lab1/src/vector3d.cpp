#include "vector3d.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
bool equalPoints(const Point3D& left, const Point3D& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}
}

Vector3D::Vector3D(const Point3D& start, const Point3D& end) : start_(start), end_(end) {}

const Point3D& Vector3D::start() const { return start_; }
const Point3D& Vector3D::end() const { return end_; }

Point3D Vector3D::displacement() const {
    return {end_.x - start_.x, end_.y - start_.y, end_.z - start_.z};
}

Vector3D Vector3D::withDisplacement(const Point3D& coordinates) const {
    return {start_, {start_.x + coordinates.x, start_.y + coordinates.y, start_.z + coordinates.z}};
}

double Vector3D::length() const {
    const Point3D coordinates = displacement();
    return std::hypot(coordinates.x, coordinates.y, coordinates.z);
}

double Vector3D::cosine(const Vector3D& other) const {
    const double leftLength = length();
    const double rightLength = other.length();
    if (leftLength == 0.0 || rightLength == 0.0) {
        throw std::domain_error("Косинус для нулевого вектора не определён.");
    }
    const Point3D left = displacement();
    const Point3D right = other.displacement();
    // Сначала нормируем координаты, чтобы не перемножать большие числа.
    const double dot = (left.x / leftLength) * (right.x / rightLength)
                     + (left.y / leftLength) * (right.y / rightLength)
                     + (left.z / leftLength) * (right.z / rightLength);
    // Округление double может вывести результат чуть за пределы диапазона.
    return std::clamp(dot, -1.0, 1.0);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    const Point3D left = displacement();
    const Point3D right = other.displacement();
    return withDisplacement({left.x + right.x, left.y + right.y, left.z + right.z});
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    const Point3D left = displacement();
    const Point3D right = other.displacement();
    return withDisplacement({left.x - right.x, left.y - right.y, left.z - right.z});
}

Vector3D Vector3D::operator*(const Vector3D& other) const {
    const Point3D left = displacement();
    const Point3D right = other.displacement();
    // Знак каждой компоненты следует из порядка сомножителей.
    return withDisplacement({left.y * right.z - left.z * right.y,
                             left.z * right.x - left.x * right.z,
                             left.x * right.y - left.y * right.x});
}

Vector3D Vector3D::operator*(double factor) const {
    const Point3D coordinates = displacement();
    return withDisplacement({coordinates.x * factor, coordinates.y * factor, coordinates.z * factor});
}

Vector3D Vector3D::operator/(double divisor) const {
    if (divisor == 0.0) {
        throw std::domain_error("Делить вектор на ноль нельзя.");
    }
    const Point3D coordinates = displacement();
    return withDisplacement({coordinates.x / divisor, coordinates.y / divisor, coordinates.z / divisor});
}

Vector3D& Vector3D::operator+=(const Vector3D& other) { return *this = *this + other; }
Vector3D& Vector3D::operator-=(const Vector3D& other) { return *this = *this - other; }
Vector3D& Vector3D::operator*=(const Vector3D& other) { return *this = *this * other; }
Vector3D& Vector3D::operator*=(double factor) { return *this = *this * factor; }
Vector3D& Vector3D::operator/=(double divisor) { return *this = *this / divisor; }

bool Vector3D::operator==(const Vector3D& other) const {
    return equalPoints(start_, other.start_) && equalPoints(end_, other.end_);
}

bool Vector3D::operator!=(const Vector3D& other) const { return !(*this == other); }
bool Vector3D::operator<(const Vector3D& other) const { return length() < other.length(); }
bool Vector3D::operator>=(const Vector3D& other) const { return length() >= other.length(); }
bool Vector3D::operator>(const Vector3D& other) const { return length() > other.length(); }
bool Vector3D::operator<=(const Vector3D& other) const { return length() <= other.length(); }

std::ostream& operator<<(std::ostream& output, const Vector3D& vector) {
    const Point3D& start = vector.start();
    const Point3D& end = vector.end();
    std::ostringstream text;
    text.imbue(output.getloc());
    // Достаточно цифр, чтобы чтение восстановило исходные значения double.
    text << std::setprecision(std::numeric_limits<double>::max_digits10)
         << start.x << ' ' << start.y << ' ' << start.z << ' '
         << end.x << ' ' << end.y << ' ' << end.z;
    return output << text.str();
}

std::istream& operator>>(std::istream& input, Vector3D& vector) {
    Point3D start;
    Point3D end;
    if (input >> start.x >> start.y >> start.z >> end.x >> end.y >> end.z) {
        vector = Vector3D(start, end);
    }
    return input;
}
