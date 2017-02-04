
#include <array>
#include <iostream>
#include <random>

#include <generator/generator.hpp>

int main()
{
    generator::generator<library::blaze, float> gen;
    auto matrix = gen.generate(generator::shape::general(3, 3), generator::property::random());
    std::cout << matrix << std::endl;

    auto matrix_positive = gen.generate(generator::shape::general(3, 3),
                                        generator::property::random(), generator::property::positive());
    std::cout << matrix_positive << std::endl;

    auto matrix_zero = gen.generate(generator::shape::general(3, 3), generator::property::zeros());
    std::cout << matrix_zero << std::endl

    auto matrix_eye = gen.generate(generator::shape::general(3, 3), generator::property::eye());
    std::cout << matrix_eye << std::endl;
}




