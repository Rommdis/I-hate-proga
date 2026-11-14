/*!
    \file my_class.cpp
    \brief Реализация CTriangle (задача 16)
*/
#include "my_class.h"
#include "resource.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace
{
constexpr double PI = 3.14159265358979323846;
constexpr double EPS = 1e-10;

bool nearlyEqual(double first, double second)
{
    return std::abs(first - second) <= EPS * std::max({1.0, std::abs(first), std::abs(second)});
}
}

double CTriangle::length(const Point& first, const Point& second)
{
    return std::hypot(first.x - second.x, first.y - second.y);
}

void CTriangle::validate() const
{
    const double coordinates[] = {a_.x, a_.y, b_.x, b_.y, c_.x, c_.y};
    for (double value : coordinates)
        if (!std::isfinite(value))
            throw MyClassException(CODE_INCORRECT_ADV_DATA, "Координаты должны быть конечными числами");

    const auto s = sideLengths();
    const double maximum = *std::max_element(s.begin(), s.end());
    const double doubled_area = std::abs((b_.x - a_.x) * (c_.y - a_.y) -
                                         (b_.y - a_.y) * (c_.x - a_.x));
    if (!std::isfinite(maximum) || !std::isfinite(doubled_area) ||
        maximum == 0.0 || doubled_area <= 1e-12 * maximum * maximum)
        throw MyClassException(CODE_INCORRECT_CONFIGURATION, "Точки не образуют невырожденный треугольник");
}

CTriangle::CTriangle() : CTriangle(0, 0, 3, 0, 0, 4)
{
}

CTriangle::CTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    : a_{x1, y1}, b_{x2, y2}, c_{x3, y3}
{
    validate();
}

double CTriangle::operator+(const CTriangle& other) const
{
    return area() + other.area();
}

double CTriangle::operator-(const CTriangle& other) const
{
    return area() - other.area();
}

void CTriangle::setVertices(double x1, double y1, double x2, double y2, double x3, double y3)
{
    // Не изменяем исходный объект, если новые вершины некорректны.
    *this = CTriangle(x1, y1, x2, y2, x3, y3);
}

CTriangle::Point CTriangle::vertexA() const { return a_; }
CTriangle::Point CTriangle::vertexB() const { return b_; }
CTriangle::Point CTriangle::vertexC() const { return c_; }

std::array<double, 3> CTriangle::sideLengths() const
{
    return {length(a_, b_), length(b_, c_), length(c_, a_)};
}

double CTriangle::perimeter() const
{
    const auto s = sideLengths();
    return std::accumulate(s.begin(), s.end(), 0.0);
}

double CTriangle::area() const
{
    return 0.5 * std::abs((b_.x - a_.x) * (c_.y - a_.y) -
                          (b_.y - a_.y) * (c_.x - a_.x));
}

std::array<double, 3> CTriangle::angles() const
{
    const auto s = sideLengths();  // AB, BC, CA
    const double ab = s[0], bc = s[1], ca = s[2];
    const auto degree = [](double opposite, double left, double right)
    {
        const double cos_value = (left * left + right * right - opposite * opposite) /
                                 (2.0 * left * right);
        return std::acos(std::clamp(cos_value, -1.0, 1.0)) * 180.0 / PI;
    };
    return {degree(bc, ab, ca), degree(ca, ab, bc), degree(ab, bc, ca)};
}

std::string CTriangle::type() const
{
    const auto s = sideLengths();
    const bool equal_ab_bc = nearlyEqual(s[0], s[1]);
    const bool equal_bc_ca = nearlyEqual(s[1], s[2]);
    const bool equal_ab_ca = nearlyEqual(s[0], s[2]);

    std::string side_type = "разносторонний";
    if (equal_ab_bc && equal_bc_ca)
        side_type = "равносторонний";
    else if (equal_ab_bc || equal_bc_ca || equal_ab_ca)
        side_type = "равнобедренный";

    auto sorted = s;
    std::sort(sorted.begin(), sorted.end());
    const double sum_squares = sorted[0] * sorted[0] + sorted[1] * sorted[1];
    const double longest_square = sorted[2] * sorted[2];
    std::string angle_type = "остроугольный";
    if (nearlyEqual(sum_squares, longest_square))
        angle_type = "прямоугольный";
    else if (sum_squares < longest_square)
        angle_type = "тупоугольный";

    return side_type + ", " + angle_type;
}

bool CTriangleSimilarity::areSimilar(const CTriangle& first, const CTriangle& second)
{
    // Используем доступ к private-полям как дружественный класс.
    std::array<double, 3> s1 = {
        CTriangle::length(first.a_, first.b_), CTriangle::length(first.b_, first.c_),
        CTriangle::length(first.c_, first.a_)
    };
    std::array<double, 3> s2 = {
        CTriangle::length(second.a_, second.b_), CTriangle::length(second.b_, second.c_),
        CTriangle::length(second.c_, second.a_)
    };
    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());
    const double ratio = s1[0] / s2[0];
    return nearlyEqual(s1[1] / s2[1], ratio) && nearlyEqual(s1[2] / s2[2], ratio);
}
