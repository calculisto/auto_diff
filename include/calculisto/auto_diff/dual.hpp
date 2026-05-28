#pragma once
#include "calculisto/array/array.hpp"
    using calculisto::array::array_t;
#include <algorithm>
#include <cmath>
#include <fmt/format.h>
    using fmt::println;

    namespace
calculisto::auto_diff
{

    namespace
detail 
{
        template <std::size_t N, class T>
        constexpr auto
    zero_array ()
    {
            auto
        r = array_t <T, N> {};
        std::ranges::fill (r, static_cast <T> (0));
        return r;
    }
} // namespace detail



    template <std::size_t N, class T = double>
    struct
dual_t
{
private:
        T
    value_m;
        array_t <T, N>
    differentials_m = detail::zero_array <N, T> ();

    dual_t (T value, array_t <T, N> const& diffs)
        : value_m { value }
        , differentials_m { diffs }
    {}
public:
        using
    value_type = T;
    dual_t ()
    {}
        explicit
    dual_t (std::size_t index)
    {
        differentials_m.at (index) = static_cast <T> (1);
    }
    dual_t (std::size_t index , T value)
        : value_m { value }
    {
        differentials_m.at (index) = static_cast <T> (1);
    }
    dual_t (std::size_t index , T value, T differential_value)
        : value_m { value }
    {
        differentials_m.at (index) = differential_value;
    }
        auto
    value () const
    {
        return value_m;
    }
        auto&
    value ()
    {
        return value_m;
    }
        auto
    differential (std::size_t index) const
    {
        return differentials_m.at (index);
    }
        auto&
    differential (std::size_t index)
    {
        return differentials_m.at (index);
    }

        friend auto
    operator <=> (dual_t const& a, dual_t const& b)
    {
        return a.value_m <=> b.value_m;
    }
        template <class U>
        friend auto
    operator <=> (dual_t const& a, U const& b)
    {
        return a.value_m <=> b;
    }
        template <class U>
        friend auto
    operator <=> (U const& a, dual_t const& b)
    {
        return a <=> b.value_m;
    }
        friend auto
    operator == (dual_t const& a, dual_t const& b)
    {
        return a.value_m == b.value_m;
    }
        template <class U>
        friend auto
    operator == (dual_t const& a, U const& b)
    {
        return a.value_m == b;
    }
        template <class U>
        friend auto
    operator == (U const& a, dual_t const& b)
    {
        return a == b.value_m;
    }

        friend auto
    operator + (dual_t a)
    {
        return a;
    }
        friend auto
    operator - (dual_t const& a)
    {
        return dual_t { -a.value_m, -a.differentials_m };
    }

        template <class U>
        friend auto
    operator + (dual_t const& a, U const& b)
    {
        return dual_t { a.value_m + b, a.differentials_m };
    }
        template <class U>
        friend auto
    operator + (U const& a, dual_t const& b)
    {
        return dual_t { a + b.value_m, b.differentials_m };
    }
        friend auto
    operator + (dual_t const& a, dual_t const& b)
    {
        return dual_t { 
              a.value_m + b.value_m
            , a.differentials_m + b.differentials_m 
        };
    }
        template <class U>
        auto&
    operator += (U const& a)
    {
        value_m += a;
        return *this;
    }

        template <class U>
        friend auto
    operator - (dual_t const& a, U const& b)
    {
        return dual_t { a.value_m - b, a.differentials_m };
    }
        template <class U>
        friend auto
    operator - (U const& a, dual_t const& b)
    {
        return dual_t { a - b.value_m, -b.differentials_m };
    }
        friend auto
    operator - (dual_t const& a, dual_t const& b)
    {
        return dual_t { 
              a.value_m - b.value_m
            , a.differentials_m - b.differentials_m 
        };
    }
        template <class U>
        auto&
    operator -= (U const& a)
    {
        value_m -= a;
        return *this;
    }

        template <class U>
        friend auto
    operator * (dual_t const& a, U const& b)
    {
        return dual_t { a.value_m * b, a.differentials_m * b };
    }
        template <class U>
        friend auto
    operator * (U const& a, dual_t const& b)
    {
        return dual_t { a * b.value_m, b.differentials_m * a };
    }
        friend auto
    operator * (dual_t const& a, dual_t const& b)
    {
        return dual_t { 
              a.value_m * b.value_m
            , a.differentials_m * b.value_m + b.differentials_m * a.value_m 
        };
    }

        template <class U>
        friend auto
    operator / (dual_t const& a, U const& b)
    {
        return dual_t { 
              a.value_m / b
            , a.differentials_m / b 
        };
    }
        template <class U>
        friend auto
    operator / (U const& a, dual_t const& b)
    {
        return dual_t { 
              a / b.value_m
            , -b.differentials_m / b.value_m / b.value_m
        };
    }
        friend auto
    operator / (dual_t const& a, dual_t const& b)
    {
        return dual_t { 
              a.value_m / b.value_m
            , (
                  a.differentials_m * b.value_m 
                - b.differentials_m * a.value_m
              ) / b.value_m / b.value_m 
        };
    }

        friend auto
    acos (dual_t const& a)
    {
            using std::acos, std::sqrt;
        return dual_t { 
              acos (a.value_m)
            , -a.differentials_m / sqrt (1. - a.value_m * a.value_m) };
    }
        friend auto
    asin (dual_t const& a)
    {
            using std::asin, std::sqrt;
        return dual_t { 
              asin (a.value_m)
            , a.differentials_m / sqrt (1. - a.value_m * a.value_m) };
    }
        friend auto
    atan (dual_t const& a)
    {
            using std::atan;
        return dual_t { 
              atan (a.value_m)
            , a.differentials_m / (1. + a.value_m * a.value_m) };
    }
        friend auto
    atan2 (dual_t const& y, dual_t const& x)
    {
            using std::atan2;
            const auto
        tmp = x.value_m * x.value_m + y.value_m * y.value_m;
        return dual_t { 
              atan2 (y.value_m, x.value_m)
            , -x.differentials_m * y.value_m / tmp 
             + y.differentials_m * x.value_m / tmp
        };
    }
        template <class U>
        friend auto
    atan2 (dual_t const& y, U const& x)
    {
            using std::atan2;
        return dual_t { 
              atan2 (y.value_m, x)
            , y.differentials_m * x / (x * x + y.value_m * y.value_m)
        };
    }
        template <class U>
        friend auto
    atan2 (U const& y, dual_t const& x)
    {
            using std::atan2;
        return dual_t { 
              atan2 (y, x.value_m)
            , -x.differentials_m * y / (x.value_m * x.value_m + y * y)
        };
    }
        friend auto
    cos (dual_t const& a)
    {
            using std::cos, std::sin;
        return dual_t { cos (a.value_m), -a.differentials_m * sin (a.value_m) };
    }
        friend auto
    sin (dual_t const& a)
    {
            using std::sin, std::cos;
        return dual_t { sin (a.value_m), a.differentials_m * cos (a.value_m) };
    }
        friend auto
    tan (dual_t const& a)
    {
            using std::tan;
            const auto
      tmp = tan (a.value_m);
        return dual_t { tmp, a.differentials_m * (1 + tmp * tmp) };
    }
        friend auto
    acosh (dual_t const& a)
    {
            using std::acosh, std::sqrt;
        return dual_t { 
              acosh (a.value_m)
            , a.differentials_m / sqrt (a.value_m * a.value_m - 1.) };
    }
        friend auto
    asinh (dual_t const& a)
    {
            using std::asinh, std::sqrt;
        return dual_t { 
              asinh (a.value_m)
            , a.differentials_m / sqrt (1. + a.value_m * a.value_m) };
    }
        friend auto
    atanh (dual_t const& a)
    {
            using std::atanh;
        return dual_t { 
              atanh (a.value_m)
            , a.differentials_m / (1. - a.value_m * a.value_m) };
    }
        friend auto
    cosh (dual_t const& a)
    {
            using std::cosh, std::sinh;
        return dual_t { cosh (a.value_m), a.differentials_m * sinh (a.value_m) };
    }
        friend auto
    sinh (dual_t const& a)
    {
            using std::sinh, std::cosh;
        return dual_t { sinh (a.value_m), a.differentials_m * cosh (a.value_m) };
    }
        friend auto
    tanh (dual_t const& a)
    {
            using std::tanh;
            const auto
        tmp = tanh (a.value_m);
        return dual_t { tmp, a.differentials_m * (1 - tmp * tmp) };
    }
        friend auto
    exp (dual_t const& a)
    {
            using std::exp;
            const auto
        tmp = exp (a.value_m);
        return dual_t { tmp, a.differentials_m * tmp };
    }
        friend auto
    exp2 (dual_t const& a)
    {
            using std::exp2;
            const auto
        tmp = exp2 (a.value_m);
        return dual_t { tmp, a.differentials_m * tmp * std::numbers::ln2 };
    }
        friend auto
    expm1 (dual_t const& a)
    {
            using std::expm1, std::exp;
        return dual_t { expm1 (a.value_m), a.differentials_m * exp (a.value_m) };
    }
    // double frexp(double value, int* exp);
    // int ilogb(double x);
    // double ldexp(double x, int exp);
        friend auto
    log (dual_t const& a)
    {
            using std::log;
        return dual_t { log (a.value_m), a.differentials_m / a.value_m };
    }
        friend auto
    log10 (dual_t const& a)
    {
            using std::log10;
        return dual_t { 
              log10 (a.value_m)
            , a.differentials_m / a.value_m / std::numbers::ln10 
        };
    }
        friend auto
    log1p (dual_t const& a)
    {
            using std::log1p;
        return dual_t { log1p (a.value_m), a.differentials_m / (a.value_m + 1.) };
    }
        friend auto
    log2 (dual_t const& a)
    {
            using std::log2;
        return dual_t { 
              log2 (a.value_m)
            , a.differentials_m / a.value_m / std::numbers::ln2 
        };
    }
    // double logb(double x);
    // double modf(double value, double* iptr);
    // double scalbn(double x, int n);
    // double scalbln(double x, long int n);
        friend auto
    cbrt (dual_t const& a)
    {
            using std::cbrt, std::pow;
        return dual_t { 
              cbrt (a.value_m)
            , a.differentials_m / 3. / pow (a.value_m, -2. / 3) 
        };
    }
        friend auto
    abs (dual_t const& a)
    {
            using std::abs, std::copysign;
        return dual_t { 
              abs (a.value_m)
            , a.differentials_m * copysign (1., a.value_m)
        };
    }
        friend auto
    fabs (dual_t const& a)
    {
            using std::fabs;
        return dual_t { 
              fabs (a.value_m)
            , a.differentials_m  * copysign (1., a.value_m)
        };
    }
        friend auto
    hypot (dual_t const& a, dual_t const& b)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b.value_m);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
            + b.differentials_m * b.value_m / tmp
        };
    }
        template <class U>
        friend auto
    hypot (dual_t const& a, U const& b)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
        };
    }
        template <class U>
        friend auto
    hypot (U const& a, dual_t const& b)
    {
            using std::hypot;
            const auto
        tmp = hypot (a, b.value_m);
        return dual_t { 
              tmp
            , b.differentials_m * b.value_m / tmp
        };
    }
        friend auto
    hypot (dual_t const& a, dual_t const& b, dual_t const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b.value_m, c.value_m);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
            + b.differentials_m * b.value_m / tmp
            + c.differentials_m * c.value_m / tmp
        };
    }
        template <class U>
        friend auto
    hypot (dual_t const& a, dual_t const& b, U const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b.value_m, c);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
            + b.differentials_m * b.value_m / tmp
        };
    }
        template <class U>
        friend auto
    hypot (dual_t const& a, U const& b, dual_t const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b, c.value_m);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
            + c.differentials_m * c.value_m / tmp
        };
    }
        template <class U>
        friend auto
    hypot (U const& a, dual_t const& b, dual_t const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a, b.value_m, c.value_m);
        return dual_t { 
              tmp
            , b.differentials_m * b.value_m / tmp
            + c.differentials_m * c.value_m / tmp
        };
    }
        template <class U, class V>
        friend auto
    hypot (U const& a, V const& b, dual_t const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a, b, c.value_m);
        return dual_t { 
              tmp
            , c.differentials_m * c.value_m / tmp
        };
    }
        template <class U, class V>
        friend auto
    hypot (dual_t const& a, U const& b, V const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a.value_m, b, c);
        return dual_t { 
              tmp
            , a.differentials_m * a.value_m / tmp
        };
    }
        template <class U, class V>
        friend auto
    hypot (U const& a, dual_t const& b, V const& c)
    {
            using std::hypot;
            const auto
        tmp = hypot (a, b.value_m, c);
        return dual_t { 
              tmp
            , b.differentials_m * b.value_m / tmp
        };
    }
        friend auto
    pow (dual_t const& a, dual_t const& b)
    {
            using std::pow, std::log;
            const auto
        tmp = pow (a.value_m, b.value_m);
        return dual_t { 
              tmp
            , b.differentials_m * tmp * log (a.value_m)
            + a.differentials_m * pow (a.value_m, b.value_m - 1) * b.value_m
        };
    }
        template <class U>
        friend auto
    pow (dual_t const& a, U const& b)
    {
            using std::pow, std::log;
            const auto
        tmp = pow (a.value_m, b);
        return dual_t { 
              tmp
            , a.differentials_m * pow (a.value_m, b - 1) * b
        };
    }
        template <class U>
        friend auto
    pow (U const& a, dual_t const& b)
    {
            using std::pow, std::log;
            const auto
        tmp = pow (a, b.value_m);
        return dual_t { 
              tmp
            , b.differentials_m * tmp * log (a)
        };
    }
        friend auto
    sqrt (dual_t const& a)
    {
            using std::sqrt, std::pow;
            const auto
        tmp = sqrt (a.value_m);
        return dual_t { 
              tmp
            , a.differentials_m / 2. / tmp
        };
    }
};
} // namespace calculisto::auto_diff


// TODO: swap

/*
namespace std {
    double acos(double x);
    double asin(double x);
    double atan(double x);
    double atan2(double y, double x);
    double cos(double x);
    double sin(double x);
    double tan(double x);
    double acosh(double x);
    double asinh(double x);
    double atanh(double x);
    double cosh(double x);
    double sinh(double x);
    double tanh(double x);
    double exp(double x);
    double exp2(double x);
    double expm1(double x);
    double frexp(double value, int* exp);
    int ilogb(double x);
    double ldexp(double x, int exp);
    double log(double x);
    double log10(double x);
    double log1p(double x);
    double log2(double x);
    double logb(double x);
    double modf(double value, double* iptr);
    double scalbn(double x, int n);
    double scalbln(double x, long int n);
    double cbrt(double x);
    // absolute values
    double abs(double j);
    double fabs(double x);
    double hypot(double x, double y);
    // three-dimensional hypotenuse
    double hypot(double x, double y, double z);
    double pow(double x, double y);
    double sqrt(double x);

-----8<----

    double erf(double x);
    double erfc(double x);
    double lgamma(double x);
    double tgamma(double x);
    double ceil(double x);
    double floor(double x);
    double nearbyint(double x);
    double rint(double x);
    long int lrint(double x);
    long long int llrint(double x);
    double round(double x);
    long int lround(double x);
    long long int llround(double x);
    double trunc(double x);
    double fmod(double x, double y);
    double remainder(double x, double y);
    double remquo(double x, double y, int* quo);
    double copysign(double x, double y);
    double nan(const char* tagp);
    double nextafter(double x, double y);
    double fdim(double x, double y);
    double fmax(double x, double y);
    double fmin(double x, double y);
    double fma(double x, double y, double z);
    // linear interpolation
    constexpr double lerp(double a, double b, double t);
    // classification / comparison functions
    int fpclassify(double x);
    bool isfinite(double x);
    bool isinf(double x);
    bool isnan(double x);
    bool isnormal(double x);
    bool signbit(double x);
    bool isgreater(double x, double y);
    bool isgreaterequal(double x, double y);
    bool isless(double x, double y);
    bool islessequal(double x, double y);
    bool islessgreater(double x, double y);
    bool isunordered(double x, double y);
    // mathematical special functions
    // associated Laguerre polynomials
    double       assoc_laguerre(unsigned n, unsigned m, double x);
    // associated Legendre functions
    double       assoc_legendre(unsigned l, unsigned m, double x);
    // beta function
    double       beta(double x, double y);
    // complete elliptic integral of the first kind
    double       comp_ellint_1(double k);
    // complete elliptic integral of the second kind
    double       comp_ellint_2(double k);
    // complete elliptic integral of the third kind
    double       comp_ellint_3(double k, double nu);
    // regular modified cylindrical Bessel functions
    double       cyl_bessel_i(double nu, double x);
    // cylindrical Bessel functions of the first kind
    double       cyl_bessel_j(double nu, double x);
    // irregular modified cylindrical Bessel functions
    double       cyl_bessel_k(double nu, double x);
    // cylindrical Neumann functions;
    // cylindrical Bessel functions of the second kind
    double       cyl_neumann(double nu, double x);
    // incomplete elliptic integral of the first kind
    double       ellint_1(double k, double phi);
    // incomplete elliptic integral of the second kind
    double       ellint_2(double k, double phi);
    // incomplete elliptic integral of the third kind
    double       ellint_3(double k, double nu, double phi);
    // exponential integral
    double       expint(double x);
    // Hermite polynomials
    double       hermite(unsigned n, double x);
    // Laguerre polynomials
    double       laguerre(unsigned n, double x);
    // Legendre polynomials
    double       legendre(unsigned l, double x);
    // Riemann zeta function
    double       riemann_zeta(double x);
    // spherical Bessel functions of the first kind
    double       sph_bessel(unsigned n, double x);
    // spherical associated Legendre functions
    double       sph_legendre(unsigned l, unsigned m, double theta);
    // spherical Neumann functions;
    // spherical Bessel functions of the second kind
    double       sph_neumann(unsigned n, double x);
}
*/
