/*!
    \file my_class.h
    \brief Класс CTriangle для работы с треугольниками (задача 16)
*/
#pragma once

#include "base_exception.h"
#include <array>
#include <string>

class MyClassException : public BaseException
{
    using BaseException::BaseException;
};

// Базовый класс для демонстрации наследования и полиморфизма.
class CTriangleBase
{
public:
    virtual ~CTriangleBase() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};

class CTriangleSimilarity;

class CTriangle : public CTriangleBase
{
public:
    struct Point
    {
        double x;
        double y;
    };

private:
    Point a_, b_, c_;
    static double length(const Point& first, const Point& second);
    void validate() const;

public:
    CTriangle();
    CTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
    CTriangle(const CTriangle& other) = default;
    ~CTriangle() override = default;
    CTriangle& operator=(const CTriangle& other) = default;

    // Результат сложения / вычитания — число (сумма / разность площадей).
    double operator+(const CTriangle& other) const;
    double operator-(const CTriangle& other) const;

    void setVertices(double x1, double y1, double x2, double y2, double x3, double y3);
    Point vertexA() const;
    Point vertexB() const;
    Point vertexC() const;
    std::array<double, 3> sideLengths() const; // AB, BC, CA
    double perimeter() const override;
    std::array<double, 3> angles() const;       // градусы при A, B, C
    double area() const override;
    std::string type() const;                    // по сторонам и углам

    friend class CTriangleSimilarity;
};

// Дружественный класс сравнивает треугольники по трём сторонам.
class CTriangleSimilarity
{
public:
    static bool areSimilar(const CTriangle& first, const CTriangle& second);
};
