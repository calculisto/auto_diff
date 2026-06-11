#include <doctest/doctest.h>
    using doctest::Approx;
#include "../include/calculisto/auto_diff/dual.hpp"
    using namespace calculisto::auto_diff;

TEST_CASE("dual.hpp")
{
SUBCASE("Construction")
{
        const auto
    a = dual_t <1> { 2 };
    CHECK(a.value == 2);
    CHECK(a.differentials[0] == 0);

        const auto
    b = dual_t <2> { 25, 0 };
    CHECK(b.value == 25);
    CHECK(b.differentials[0] == 1);
    CHECK(b.differentials[1] == 0);

        const auto
    c = dual_t <2> { 32, 1, 0.5 };
    CHECK(c.value == 32);
    CHECK(c.differentials[0] == 0);
    CHECK(c.differentials[1] == 0.5);
}
SUBCASE("A dual with no differentials behaves normal number")
{
        const auto
    x = dual_t <1> { 3, 0 };
        const auto
    p1 = 2 * x;
    CHECK(p1.value == 6);
    CHECK(p1.differentials[0] == 2);

        const auto
    a = dual_t <1> { 2 };
        const auto
    p2 = a * x;
    CHECK(p2.value == 6);
    CHECK(p2.differentials[0] == 2);
}
SUBCASE("Casting from underlying type")
{
    // This is used to initialize a sum
        const auto
    x =static_cast <dual_t <3>> (0);
    CHECK(x.value == 0);
        const auto
    y =static_cast <dual_t <3>> (1);
    CHECK(y.value == 1);
}
SUBCASE("Arithmetic, one value")
{
        const auto
    x = dual_t <1> { 2, 0 };
    CHECK((+x).value == 2);
    CHECK((-x).value == -2);
    CHECK(x == 2);
    CHECK(2 == x);
    CHECK(x != 3);
    CHECK(3 != x);
    CHECK(x <  3);
    CHECK(1 <  x);
    CHECK(x >  1);
    CHECK(3 >  x);
    CHECK(x <= 3);
    CHECK(1 <= x);
    CHECK(x >= 1);
    CHECK(3 >= x);
    CHECK(x <= 2);
    CHECK(2 <= x);
    CHECK(x >= 2);
    CHECK(2 >= x);
    {
            const auto
        f = x + 1;
        CHECK(f.value == 3);
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = 1 + x;
        CHECK(f.value == 3);
        CHECK(f.differentials[0] == 1);
    }
    {
            auto
        f = dual_t <1> { 1, 0 };
        f += 1;
        CHECK(f.value == 2);
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = x - 1;
        CHECK(f.value == 1);
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = 1 - x;
        CHECK(f.value == -1);
        CHECK(f.differentials[0] == -1);
    }
    {
            auto
        f = dual_t <1> { 1, 0 };
        f -= 1;
        CHECK(f.value == 0);
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = x * 2;
        CHECK(f.value == 4);
        CHECK(f.differentials[0] == 2);
    }
    {
            const auto
        f = 2 * x;
        CHECK(f.value == 4);
        CHECK(f.differentials[0] == 2);
    }
    {
            const auto
        f = x / 2;
        CHECK(f.value == 1);
        CHECK(f.differentials[0] == .5);
    }
    {
            const auto
        f = 1 / x;
        CHECK(f.value == .5);
        CHECK(f.differentials[0] == -0.25);
    }
}
SUBCASE("Arithmetic, two values")
{
        const auto
    x = dual_t <2> { 2, 0 };
        const auto
    y = dual_t <2> { 3, 1 };
    CHECK(x != y);
    CHECK(x <  y);
    CHECK(y >  x);
    CHECK(x <= y);
    CHECK(y >= x);
    {
            const auto
        f = x + y;
        CHECK(f.value == 5);
        CHECK(f.differentials[0] == 1);
        CHECK(f.differentials[1] == 1);
    }
    {
            const auto
        f = x - y;
        CHECK(f.value == -1);
        CHECK(f.differentials[0] == 1);
        CHECK(f.differentials[1] == -1);
    }
    {
            const auto
        f = x * y;
        CHECK(f.value == 6);
        CHECK(f.differentials[0] == 3);
        CHECK(f.differentials[1] == 2);
    }
    {
            const auto
        f = x / y;
        CHECK(f.value == 2. / 3);
        CHECK(f.differentials[0] == 1. / 3);
        CHECK(f.differentials[1] == -2. / 9);
    }
}
SUBCASE("Functions, one value")
{
        const auto
    x = dual_t <1> { 0.5, 0 };
    {
            const auto
        f = acos (x);
        CHECK(f.value == std::acos (x.value));
        CHECK(f.differentials[0] == -1 / sqrt (1 - pow (x.value, 2)));
    }
    {
            const auto
        f = asin (x);
        CHECK(f.value == std::asin (x.value));
        CHECK(f.differentials[0] == 1 / sqrt (1 - pow (x.value, 2)));
    }
    {
            const auto
        f = atan (x);
        CHECK(f.value == std::atan (x.value));
        CHECK(f.differentials[0] == 1 / (1 + pow (x.value, 2)));
    }
    {
            const auto
        f = cos (x);
        CHECK(f.value == std::cos (x.value));
        CHECK(f.differentials[0] == -std::sin (x.value));
    }
    {
            const auto
        f = sin (x);
        CHECK(f.value == std::sin (x.value));
        CHECK(f.differentials[0] == std::cos (x.value));
    }
    {
            const auto
        f = tan (x);
        CHECK(f.value == std::tan (x.value));
        CHECK(f.differentials[0] == 1 + f.value * f.value);
    }
        const auto
    y = dual_t <1> { 2, 0 };
    {
            const auto
        f = acosh (y);
        CHECK(f.value == std::acosh (y.value));
        CHECK(f.differentials[0] == 1 / sqrt (pow (y.value, 2) - 1));
    }
    {
            const auto
        f = asinh (y);
        CHECK(f.value == std::asinh (y.value));
        CHECK(f.differentials[0] == 1 / sqrt (pow (y.value, 2) + 1));
    }
    {
            const auto
        f = atanh (x);
        CHECK(f.value == std::atanh (x.value));
        CHECK(f.differentials[0] == 1 / (1 - pow (x.value, 2)));
    }
    {
            const auto
        f = cosh (x);
        CHECK(f.value == std::cosh (x.value));
        CHECK(f.differentials[0] == std::sinh (x.value));
    }
    {
            const auto
        f = sinh (x);
        CHECK(f.value == std::sinh (x.value));
        CHECK(f.differentials[0] == std::cosh (x.value));
    }
    {
            const auto
        f = tanh (x);
        CHECK(f.value == std::tanh (x.value));
        CHECK(f.differentials[0] == 1 - f.value * f.value);
    }
    {
            const auto
        f = exp (x);
        CHECK(f.value == std::exp (x.value));
        CHECK(f.differentials[0] == f.value);
    }
    {
            const auto
        f = exp2 (x);
        CHECK(f.value == std::exp2 (x.value));
        CHECK(f.differentials[0] == f.value * std::numbers::ln2);
    }
    {
            const auto
        f = expm1 (x);
        CHECK(f.value == std::expm1 (x.value));
        CHECK(f.differentials[0] == f.value + 1);
    }
    {
            const auto
        f = log (x);
        CHECK(f.value == std::log (x.value));
        CHECK(f.differentials[0] == 1 / x.value);
    }
    {
            const auto
        f = log10 (x);
        CHECK(f.value == std::log10 (x.value));
        CHECK(f.differentials[0] == 1 / x.value / std::numbers::ln10);
    }
    {
            const auto
        f = log1p (x);
        CHECK(f.value == std::log1p (x.value));
        CHECK(f.differentials[0] == 1 / (x.value + 1));
    }
    {
            const auto
        f = log2 (x);
        CHECK(f.value == std::log2 (x.value));
        CHECK(f.differentials[0] == 1 / x.value / std::numbers::ln2);
    }
    {
            const auto
        f = cbrt (x);
        CHECK(f.value == std::cbrt (x.value));
        CHECK(f.differentials[0] == 1. / 3 / pow (x.value, -2. / 3));
    }
    {
            const auto
        f = abs (x);
        CHECK(f.value == std::abs (x.value));
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = fabs (x);
        CHECK(f.value == std::fabs (x.value));
        CHECK(f.differentials[0] == 1);
    }
    {
            const auto
        f = sqrt (x);
        CHECK(f.value == std::sqrt (x.value));
        CHECK(f.differentials[0] == 1. / 2 / f.value);
    }
}
SUBCASE("Functions, two values")
{
        const auto
    x = dual_t <2> { 2, 0 };
        const auto
    y = dual_t <2> { 3, 1 };
    {
            const auto
        f = atan2 (y, x);
        CHECK(f.value == std::atan2 (y.value, x.value));
        CHECK(f.differentials[0] == -3. / (4 + 9));
        CHECK(f.differentials[1] == 2. / (4 + 9));
    }
    {
            const auto
        f = atan2 (y, 2);
        CHECK(f.value == std::atan2 (y.value, 2));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == 2. / (4 + 9));
    }
    {
            const auto
        f = atan2 (3, x);
        CHECK(f.value == std::atan2 (3, x.value));
        CHECK(f.differentials[0] == -3. / (4 + 9));
        CHECK(f.differentials[1] == 0);
    }
    {
            const auto
        f = hypot (x, y);
        CHECK(f.value == std::hypot (x.value, y.value));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == y.value / f.value);
    }
    {
            const auto
        f = hypot (x, 1);
        CHECK(f.value == std::hypot (x.value, 1));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == 0);
    }
    {
            const auto
        f = hypot (2, y);
        CHECK(f.value == std::hypot (2, y.value));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == y.value / f.value);
    }
    {
            const auto
        f = pow (x, y);
        CHECK(f.value == std::pow (x.value, y.value));
        CHECK(f.differentials[0] == y.value * pow (x.value, y.value - 1));
        CHECK(f.differentials[1] == f.value * std::log (x.value));
    }
    {
            const auto
        f = pow (x, 3);
        CHECK(f.value == std::pow (x.value, 3));
        CHECK(f.differentials[0] == 3 * pow (x.value, 2));
        CHECK(f.differentials[1] == 0);
    }
    {
            const auto
        f = pow (2, y);
        CHECK(f.value == std::pow (x.value, y.value));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == f.value * std::log (x.value));
    }
}
SUBCASE("Functions, three values")
{
        const auto
    x = dual_t <3> { 2, 0 };
        const auto
    y = dual_t <3> { 1, 1 };
        const auto
    z = dual_t <3> { 3, 2 };
    {
            const auto
        f = hypot (x, y, z);
        CHECK(f.value == std::hypot (x.value, y.value, z.value));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == y.value / f.value);
        CHECK(f.differentials[2] == z.value / f.value);
    }{
            const auto
        f = hypot (2, y, z);
        CHECK(f.value == std::hypot (2, y.value, z.value));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == y.value / f.value);
        CHECK(f.differentials[2] == z.value / f.value);
    }{
            const auto
        f = hypot (x, 1, z);
        CHECK(f.value == std::hypot (x.value, 1, z.value));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == 0);
        CHECK(f.differentials[2] == z.value / f.value);
    }{
            const auto
        f = hypot (x, y, 3);
        CHECK(f.value == std::hypot (x.value, y.value, 3));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == y.value / f.value);
        CHECK(f.differentials[2] == 0);
    }{
            const auto
        f = hypot (2, 1, z);
        CHECK(f.value == std::hypot (2, 1, z.value));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == 0);
        CHECK(f.differentials[2] == z.value / f.value);
    }{
            const auto
        f = hypot (x, 1, 3);
        CHECK(f.value == std::hypot (x.value, 1, 3));
        CHECK(f.differentials[0] == x.value / f.value);
        CHECK(f.differentials[1] == 0);
        CHECK(f.differentials[2] == 0);
    }{
            const auto
        f = hypot (2, y, 3);
        CHECK(f.value == std::hypot (2, y.value, 3));
        CHECK(f.differentials[0] == 0);
        CHECK(f.differentials[1] == y.value / f.value);
        CHECK(f.differentials[2] == 0);
    }
}
SUBCASE("Polynomials")
{
        const auto
    x = dual_t <3> { 2, 0 };
        const auto
    y = dual_t <3> { 3, 1 };
        const auto
    z = dual_t <3> { 4, 2 };
        const auto
    f = 3 * pow (x, 3) + 4 * pow (y, 2) + 5 * z + 3;
    CHECK(f.value == 83);
    CHECK(f.differentials[0] == 36);
    CHECK(f.differentials[1] == 24);
    CHECK(f.differentials[2] == 5);
}
SUBCASE("Unused dimensions")
{
        const auto
    x = dual_t <3> { 2, 0 };
        const auto
    y = dual_t <3> { 3, 1 };
        const auto
    z = 4.;
        const auto
    f = 3 * pow (x, 3) + 4 * pow (y, 2) + 5 * z + 3;
    CHECK(f.value == 83);
    CHECK(f.differentials[0] == 36);
    CHECK(f.differentials[1] == 24);
    CHECK(f.differentials[2] == 0);
}
    
} // TEST_CASE("dual.hpp")
