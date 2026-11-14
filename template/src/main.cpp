/*!
    \file main.cpp
    \brief Интерфейс работы с CTriangle (задача 16)
*/
#include "interface.h"
#include "resource.h"
#include "my_class.h"

#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, [[maybe_unused]] char* argv[])
{
    DEBUG_LOGGING("Run DEBUG MODE");

    Interface* menu = new Interface();
    std::vector<CTriangle> impl;

#ifdef SELFTEST
    DEBUG_LOGGING("Run SELFTEST MODE");
    if (argc != 2)
    {
        ERR_LOGGING(CODE_SELFTEST_ERROR, "SELFTEST invalid input");
        delete menu;
        return CODE_SELFTEST_ERROR;
    }
    try { menu->set(argv[1]); }
    catch (InterfaceException& ex)
    {
        ERR_LOGGING(ex.res(), ex.msg());
        delete menu;
        return ex.res();
    }
#else
    if (argc > 2)
    {
        ERR_LOGGING(CODE_INCORRECT_CONFIGURATION, "UI invalid input");
        delete menu;
        return CODE_INCORRECT_CONFIGURATION;
    }
#endif

    // Порядок чтения координат намеренно фиксирован: A, B, C.
    auto readTriangle = [&menu]() -> CTriangle
    {
#ifndef SELFTEST
        std::cout << "Введите x1 y1 x2 y2 x3 y3 (через пробел или с новой строки): " << std::endl;
#endif
        const double x1 = menu->read_ex<double>();
        const double y1 = menu->read_ex<double>();
        const double x2 = menu->read_ex<double>();
        const double y2 = menu->read_ex<double>();
        const double x3 = menu->read_ex<double>();
        const double y3 = menu->read_ex<double>();
        return CTriangle(x1, y1, x2, y2, x3, y3);
    };

    auto last = [&impl]() -> CTriangle&
    {
        if (impl.empty())
            throw MyClassException(CODE_INCORRECT_CONFIGURATION,
                                   "Сначала создайте треугольник командой 1");
        return impl.back();
    };

    auto lastTwo = [&impl]()
    {
        if (impl.size() < 2)
            throw MyClassException(CODE_INCORRECT_CONFIGURATION,
                                   "Для операции нужны два треугольника (команда 1)");
    };

    auto printTriangle = [](const CTriangle& triangle)
    {
        const CTriangle::Point a = triangle.vertexA();
        const CTriangle::Point b = triangle.vertexB();
        const CTriangle::Point c = triangle.vertexC();
        const auto sides = triangle.sideLengths();
        const auto angles = triangle.angles();
        const CTriangleBase& figure = triangle;  // полиморфный вызов виртуальных методов
        LOGGING("A(" << a.x << ", " << a.y << ")  B(" << b.x << ", " << b.y
                << ")  C(" << c.x << ", " << c.y << ")");
        LOGGING("Стороны AB, BC, CA: " << sides[0] << ", " << sides[1] << ", " << sides[2]);
        LOGGING("Периметр: " << figure.perimeter());
        LOGGING("Углы A, B, C (градусы): " << angles[0] << ", " << angles[1] << ", " << angles[2]);
        LOGGING("Тип: " << triangle.type());
        LOGGING("Площадь: " << figure.area());
    };

    std::cout << std::setprecision(10);
    while (true)
    {
        try
        {
#ifndef SELFTEST
            menu->show();
            menu->invite();
#endif
            switch (menu->read())
            {
            case INTERFACE_COMMAND_1:
                impl.push_back(readTriangle());
                LOGGING("Треугольник создан. Всего: " << impl.size());
                continue;
            case INTERFACE_COMMAND_2:
                last();
                impl.pop_back();
                LOGGING("Последний треугольник удалён. Всего: " << impl.size());
                continue;
            case INTERFACE_COMMAND_3:
                printTriangle(last());
                continue;
            case INTERFACE_COMMAND_5:
            {
                CTriangle& triangle = last();
                const CTriangle updated = readTriangle();
                const auto a = updated.vertexA();
                const auto b = updated.vertexB();
                const auto c = updated.vertexC();
                triangle.setVertices(a.x, a.y, b.x, b.y, c.x, c.y);
                LOGGING("Координаты последнего треугольника изменены");
                continue;
            }
            case INTERFACE_COMMAND_6:
            {
                const auto sides = last().sideLengths();
                LOGGING("AB = " << sides[0] << ", BC = " << sides[1] << ", CA = " << sides[2]);
                continue;
            }
            case INTERFACE_COMMAND_7:
                LOGGING("Периметр = " << last().perimeter());
                continue;
            case INTERFACE_COMMAND_8:
            {
                const auto angles = last().angles();
                LOGGING("Углы (градусы): " << angles[0] << ", " << angles[1] << ", " << angles[2]);
                continue;
            }
            case INTERFACE_COMMAND_9:
                LOGGING("Тип: " << last().type());
                continue;
            case INTERFACE_COMMAND_10:
                LOGGING("Площадь = " << last().area());
                continue;
            case INTERFACE_COMMAND_11:
                lastTwo();
                LOGGING("Сумма площадей = " << (impl[impl.size() - 2] + impl.back()));
                continue;
            case INTERFACE_COMMAND_12:
                lastTwo();
                LOGGING("Разность площадей (предпоследний - последний) = "
                        << (impl[impl.size() - 2] - impl.back()));
                continue;
            case INTERFACE_COMMAND_13:
                lastTwo();
                LOGGING("Подобие: " << (CTriangleSimilarity::areSimilar(impl[impl.size() - 2], impl.back())
                        ? "треугольники подобны" : "треугольники не подобны"));
                continue;
            case INTERFACE_COMMAND_14:
                LOGGING("Количество треугольников: " << impl.size());
                for (std::size_t i = 0; i < impl.size(); ++i)
                {
                    LOGGING("Треугольник " << i + 1 << ":");
                    printTriangle(impl[i]);
                }
                continue;
            case INTERFACE_COMMAND_EXIT:
                break;
            case INTERFACE_COMMAND_BREAK:
            case INTERFACE_COMMAND_INVALID:
            default:
                throw InterfaceException(CODE_INCORRECT_CONFIGURATION, "Некорректная команда");
            }
        }
        catch (InterfaceException& ex)
        {
#ifdef SELFTEST
            ERR_LOGGING(CODE_SELFTEST_ERROR, ex.msg());
            impl.clear();
            delete menu;
            return CODE_SELFTEST_ERROR;
#else
            ERR_LOGGING(ex.res(), ex.msg());
            continue;
#endif
        }
        catch (MyClassException& ex)
        {
#ifdef SELFTEST
            ERR_LOGGING(CODE_SELFTEST_ERROR, ex.msg());
            impl.clear();
            delete menu;
            return CODE_SELFTEST_ERROR;
#else
            ERR_LOGGING(ex.res(), ex.msg());
            continue;
#endif
        }
        break;
    }

    impl.clear();
    delete menu;
    return CODE_NO_ERROR;
}
