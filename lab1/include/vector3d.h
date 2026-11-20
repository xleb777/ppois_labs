#ifndef LAB1_VECTOR3D_H
#define LAB1_VECTOR3D_H

#include <iosfwd>

/// Координаты одной точки в трёхмерном пространстве.
struct Point3D {
    double x = 0.0; ///< Координата по оси X.
    double y = 0.0; ///< Координата по оси Y.
    double z = 0.0; ///< Координата по оси Z.
};

/// Вектор, заданный начальной и конечной точками.
class Vector3D {
public:
    /// Создаёт нулевой вектор в начале координат.
    Vector3D() = default;
    /// Создаёт вектор из двух точек.
    Vector3D(const Point3D& start, const Point3D& end);
    /// Копирует обе точки.
    Vector3D(const Vector3D&) = default;
    /// Заменяет обе точки копиями точек другого вектора.
    Vector3D& operator=(const Vector3D&) = default;
    /// У класса нет ресурсов, требующих ручного освобождения.
    ~Vector3D() = default;

    /// Возвращает начальную точку.
    const Point3D& start() const;
    /// Возвращает конечную точку.
    const Point3D& end() const;
    /// Возвращает длину перемещения из начала в конец.
    double length() const;
    /// Возвращает косинус угла. Для нулевого вектора бросает std::domain_error.
    double cosine(const Vector3D& other) const;

    /// Складывает перемещения, сохраняя начальную точку левого операнда.
    Vector3D operator+(const Vector3D& other) const;
    /// Вычитает перемещения, сохраняя начальную точку левого операнда.
    Vector3D operator-(const Vector3D& other) const;
    /// Вычисляет векторное произведение, сохраняя начало левого операнда.
    Vector3D operator*(const Vector3D& other) const;
    /// Умножает перемещение на число, сохраняя начало.
    Vector3D operator*(double factor) const;
    /// Делит перемещение на число. Для нуля бросает std::domain_error.
    Vector3D operator/(double divisor) const;

    /// Прибавляет перемещение другого вектора.
    Vector3D& operator+=(const Vector3D& other);
    /// Вычитает перемещение другого вектора.
    Vector3D& operator-=(const Vector3D& other);
    /// Заменяет перемещение векторным произведением.
    Vector3D& operator*=(const Vector3D& other);
    /// Умножает перемещение на число.
    Vector3D& operator*=(double factor);
    /// Делит перемещение на число.
    Vector3D& operator/=(double divisor);

    /// Проверяет точное совпадение обеих точек.
    bool operator==(const Vector3D& other) const;
    /// Проверяет, различается ли хотя бы одна точка.
    bool operator!=(const Vector3D& other) const;
    /// Проверяет, строго ли меньше длина.
    bool operator<(const Vector3D& other) const;
    /// Проверяет, больше ли длина или равна.
    bool operator>=(const Vector3D& other) const;
    /// Проверяет, строго ли больше длина.
    bool operator>(const Vector3D& other) const;
    /// Проверяет, меньше ли длина или равна.
    bool operator<=(const Vector3D& other) const;

private:
    Point3D start_;
    Point3D end_;

    Point3D displacement() const;
    Vector3D withDisplacement(const Point3D& coordinates) const;
};

/// Записывает шесть координат в порядке x1 y1 z1 x2 y2 z2.
std::ostream& operator<<(std::ostream& output, const Vector3D& vector);
/// Читает шесть координат. При ошибке исходный вектор не меняется.
std::istream& operator>>(std::istream& input, Vector3D& vector);

#endif
