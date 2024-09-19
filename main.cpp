#include "xvar.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <stdio.h>

#define test(exp, name) \
   if (exp) {\
       std::cout<< "test " << (name) << " passed" << std::endl;\
   } else {\
       std::string msg = "test "; \
       msg += name;\
       msg += " failed";\
       std::cout<< msg << std::endl;\
       assert(false);\
   }


void test_x_f0(){
    xvar<int> x = x_f0(int, 3);
    std::cout<< "x:" << x() << std::endl;
    test(x() == 3, "f0 test 1");
}

void test_x_f1(){
    {
        xvar<int> x = x_f0(int, 3);
        std::cout<< "x:" << x() << std::endl;
        test(x() == 3, "f1 test 1.1");

        xvar<int> y = x_f1(int, x*x, x);
        std::cout<< "y:" << y() << std::endl;
        test(y() == 9, "f1 test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(4);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 16, "f1 test 1.2.2");
    }

    std::cout<< "===========" << std::endl;

    {
        xvar<int> x = x_f0(int, 3);
        std::cout<< "x:" << x() << std::endl;
        test(x() == 3, "f1 test 2.1");

        xvar<std::string> y = x_f1(std::string, std::to_string(x) + "_kkk", x);
        std::cout<< "y:" << y() << std::endl;
        test(y() == "3_kkk", "f1 test 2.1.1");

        std::cout<< "***********" << std::endl;
        x.setValue(4);

        std::cout<< "y:" << y() << std::endl;
        test(y() == "4_kkk", "f1 test 2.2.2");
    }
}

void test_x_f2(){
    {
        xvar<int> x = x_f0(int, 3);
        xvar<int> y = x_f0(int, 4);
        std::cout<< "x:" << x() << " y:" << y() << std::endl;
        test(x() == 3, "f2 test 1.1.1");
        test(y() == 4, "f2 test 1.1.2");


        xvar<int> z = x_f2(int, x+y, x, y);
        std::cout<< "z:" << z() << std::endl;
        test(z() == 7, "f2 test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "y:" << z() << std::endl;
        test(z() == 9, "f2 test 1.2.2");
    }

    std::cout<< "===========" << std::endl;
}

void test_x_f2_auto(){
    {
        auto x = x_f0(int, 3);
        auto y = x_f0(int, 4);
        std::cout<< "x:" << x() << " y:" << y() << std::endl;
        test(x() == 3, "f2 auto test 1.1.1");
        test(y() == 4, "f2 auto test 1.1.2");

        auto z = x_f2(int, x+y, x, y);
        std::cout<< "z:" << z() << std::endl;
        test(z() == 7, "f2 auto test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 9, "f2 auto test 1.2.2");
    }

    std::cout<< "===========" << std::endl;
}

void test_x_operator_add(){
    {
        auto x = x_f0(int, 3);
        auto y = x_f0(int, 4);
        std::cout<< "x:" << x() << " y:" << y() << std::endl;
        test(x() == 3, "operator_add test 1.1.1");
        test(y() == 4, "operator_add test 1.1.2");

        auto z = x + y;
        std::cout<< "z:" << z() << std::endl;
        test(z() == 7, "operator_add test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 9, "operator_add test 1.2.2");
    }

    std::cout<< "===========" << std::endl;

    {
        auto x = x_f0(int, 3);
        auto y = x_f0(double, 0.5);
        std::cout<< "x:" << x() << " y:" << y() << std::endl;
        test(x() == 3, "operator_add test 1.1.1");
        test(y() == 0.5, "operator_add test 1.1.2");

        xvar<double> z = x + y;
        std::cout<< "z:" << z() << std::endl;
        test(z() == 3.5, "operator_add test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 5.5, "operator_add test 1.2.2");
    }

    std::cout<< "===========" << std::endl;

    {
        auto x = x_f0(int, 3);
        xvar<double> z = x + 0.5;
        std::cout<< "z:" << z() << std::endl;
        test(z() == 3.5, "operator_add test 2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(4);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 4.5, "operator_add test 2.2");
    }

    std::cout<< "===========" << std::endl;

    {
        auto x = x_f0(int, 3);
        xvar<double> z = 0.5 + x;
        std::cout<< "z:" << z() << std::endl;
        test(z() == 3.5, "operator_add test 3.1");

        std::cout<< "***********" << std::endl;
        x.setValue(4);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 4.5, "operator_add test 3.2");
    }

    std::cout<< "===========" << std::endl;
}

void test_x_operator_unm(){
    {
        auto x = x_f0(int, 3);
        std::cout<< "x:" << x() << std::endl;
        test(x() == 3, "operator_unm test 1.1.1");

        auto y = -x;
        std::cout<< "y:" << y() << std::endl;
        test(y() == -3, "operator_unm test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "y:" << y() << std::endl;
        test(y() == -5, "operator_unm test 1.2.2");

        // x_fn(int, x*y+z - 9 *w, x, y, z, w)
    }

    std::cout<< "===========" << std::endl;
}

void test_x_const_lift(){
    {
        auto x = x_f0(int, 3);
        std::cout<< "x:" << x() << std::endl;
        test(x() == 3, "operator_const_lift test 1.1.1");

        auto z = x + 4;
        std::cout<< "z:" << z() << std::endl;
        test(z() == 7, "operator_const_lift test 1.2.1");

        std::cout<< "***********" << std::endl;
        x.setValue(5);

        std::cout<< "z:" << z() << std::endl;
        test(z() == 9, "operator_const_lift test 1.2.2");
    }

    std::cout<< "===========" << std::endl;
}

template <typename T, typename S1>
static xvar<T> f1(const std::function<T(S1)>& f, xvar_value<S1>* s1) {
    return xvar<T>(xvar_f1<T, S1>::create(f, s1));
}

template <typename T, typename ...Sn>
static xvar<T> fn(const std::function<T(Sn...)>& f, xvar_value<Sn>... args) {
    return xvar<T>(xvar_fn<T, Sn...>::create(f, args...));
}

void test_x_fn() {
    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 1);
        xvar<char> x3 = x_f0(char, 1);
        xvar<short> x4 = x_f0(short, 1);
        xvar<long> x5 = x_f0(long, 1);

        typedef double T;
        auto y =  xvar<T>(xvar_fn<T, decltype(x1)::ValueType, decltype(x2)::ValueType,
                decltype(x3)::ValueType, decltype(x4)::ValueType, decltype(x5)::ValueType>::create\
                ([=](decltype(x1)::ValueType x1, decltype(x2)::ValueType x2, decltype(x3)::ValueType x3,
                     decltype(x4)::ValueType x4, decltype(x5)::ValueType x5)-> T {
                 return x1 + x2 + x3 + x4 + x5;
                 }, x1.p(), x2.p(), x3.p(), x4.p(), x5.p()));


        std::cout<< "y:" << y() << std::endl;
        test(y() == 5.0, "x_fn test 1.1");
        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(x5()*2);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 10.0, "x_fn test 1.2");
    }

    {
        typedef double T;

        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 1);
        xvar<char> x3 = x_f0(char, 1);
        xvar<short> x4 = x_f0(short, 1);
        xvar<long> x5 = x_f0(long, 1);

        auto y = x_tuple(x1, x2, x3, x4, x5).link<T>([=](auto x1, auto x2, auto x3, auto x4, auto x5)
                {return x1 + x2 + x3 + x4 + x5; });


        std::cout<< "y:" << y() << std::endl;
        test(y() == 5.0, "x_fn test 2.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(x5()*2);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 10.0, "x_fn test 2.2");

        std::cout<< "***********" << std::endl;

        auto z = x_form<T>(x1, x2, x3, x4, x5) >> [=](auto x1, auto x2, auto x3, auto x4, auto x5)
                {return x1 + x2 + x3 + x4 + x5; };

        // auto z =  x1 + x2 + x3 + x4 + x5;
        // auto z = x_fn(T, x1 + x2 + x3 + x4 + x5);

        std::cout<< "y:" << y() << std::endl;
        test(z() == 10.0, "x_fn test 2.3");

        std::cout<< "***********" << std::endl;
        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(x5()*2);

        std::cout<< "y:" << z() << std::endl;
        test(z() == 20.0, "x_fn test 2.4");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        // xvar<short> x4 = x_f0(short, 5);
        // xvar<long> x5 = x_f0(long, 5);

        auto y = x_f3(double, x1*x1 + x2*2 + x3 *3 + 3, x1, x2, x3);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 17.0, "x_fn test 3.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        // x4.setValue(x4()*2);
        // x5.setValue(x5()*2);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 33.0, "x_fn test 3.2");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);

        auto y = x_f4(double, x1*x1 + x2*2 + x3 *3 + 3 - x4, x1, x2, x3, x4);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 12.0, "x_fn test 4.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 23.0, "x_fn test 4.2");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);

        // 1*1 + 2*2 + 3*3 + 3 - 5 + 3*4*4
        // 1 +４ + 9 + 3 -5 + 48
        auto y = x_f5(double, x1*x1 + x2*2 + x3 *3 + 3 - x4 + 3*4*x5, x1, x2, x3, x4, x5);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 60.0, "x_fn test 5.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());

        // 2*2 + 4*2 + 6*3 + 3 - 10 + 3*4*(-4)
        // 4 +8 + 18 + 3 -10 - 48
        std::cout<< "y:" << y() << std::endl;
        test(y() == -25.0, "x_fn test 5.2");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);
        xvar<double> x6 = x_f0(double, 2.5);

        // 1*1 + 2*2 + 3*3 + 3 - 5 + 3*4*4 + 2.5
        // 1 +４ + 9 + 3 -5 + 48 + 2.5
        auto y = x_f6(double, x1*x1 + x2*2 + x3 *3 + 3 - x4 + 3*4*x5 + x6, x1, x2, x3, x4, x5, x6);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 62.5, "x_fn test 6.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());
        x6.setValue(x6()*2);

        // 2*2 + 4*2 + 6*3 + 3 - 10 + 3*4*(-4) + 5.0
        // 4 +8 + 18 + 3 -10 - 48 + 5.0
        std::cout<< "y:" << y() << std::endl;
        test(y() == -20.0, "x_fn test 6.2");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);
        xvar<double> x6 = x_f0(double, 2.5);
        xvar<double> x7 = x_f0(double, 0.5);

        // 1*1 + 2*2 + 3*3 + 3 - 5 + 3*4*4 + 2.5 - 0.5
        // 1 +４ + 9 + 3 -5 + 48 + 2.5 - 0.5
        auto y = x_f7(double, x1*x1 + x2*2 + x3 *3 + 3 - x4 + 3*4*x5 + x6 - x7, x1, x2, x3, x4, x5, x6, x7);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 62.0, "x_fn test 7.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());
        x6.setValue(x6()*2);
        x7.setValue(x7()*3);

        // 2*2 + 4*2 + 6*3 + 3 - 10 + 3*4*(-4) + 5.0 - 1.5
        // 4 +8 + 18 + 3 -10 - 48 + 5.0 - 1.5
        std::cout<< "y:" << y() << std::endl;
        test(y() == -21.5, "x_fn test 7.2");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);
        xvar<double> x6 = x_f0(double, 2.5);
        xvar<double> x7 = x_f0(double, 0.5);
        xvar<double> x8 = x_f0(double, -0.5);

        // 1*1 + 2*2 + 3*3 + 3 - 5 + 3*4*4 + 2.5 - 0.5 + 2*(-0.5)
        // 1 +４ + 9 + 3 -5 + 48 + 2.5 - 0.5 - 1.0
        auto y = x_f8(double, x1*x1 + x2*2 + x3 *3 + 3 - x4 + 3*4*x5 + x6 - x7 + 2*x8, x1, x2, x3, x4, x5, x6, x7, x8);

        std::cout<< "y:" << y() << std::endl;
        test(y() == 61.0, "x_fn test 7.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());
        x6.setValue(x6()*2);
        x7.setValue(x7()*3);
        x8.setValue(x8()*2);

        // 2*2 + 4*2 + 6*3 + 3 - 10 + 3*4*(-4) + 5.0 - 1.5 + 2*(-1.0)
        // 4 +8 + 18 + 3 -10 - 48 + 5.0 - 1.5 - 2.0
        std::cout<< "y:" << y() << std::endl;
        test(y() == -23.5, "x_fn test 7.2");
    }
}

void test_x_exp(){
    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);
        xvar<double> x6 = x_f0(double, 2.5);
        xvar<double> x7 = x_f0(double, 0.5);
        xvar<double> x8 = x_f0(double, -0.5);

        auto y = x1 * x2 + x3*3 - x4*x5*x6 - x7 / x8;
        std::cout<< "y:" << y() << std::endl;
        //   y = 1 * 2 + 3 * 3 - 5*4*2.5 + 0.5 / 0.5
        test(y() == -38.0, "x_exp test 1.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());
        x6.setValue(x6()*2);
        x7.setValue(x7()*3);
        x8.setValue(x8()*2);
        std::cout<< "y:" << y() << std::endl;

        test(y() == 227.5, "x_exp test 1.1");
    }

    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);

        auto y = x1 * x2 < x3 + x4;
        std::cout<< "y:" << y() << std::endl;
        test(y() == true, "x_exp test 2.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*8);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);

        std::cout<< "y:" << y() << std::endl;
        test(y() == false, "x_exp test 2.1");
    }
}

void test_x_fn_foreach(){
    {
        xvar<double> x1 = x_f0(double, 1);
        xvar<int> x2 = x_f0(int, 2);
        xvar<char> x3 = x_f0(char, 3);
        xvar<short> x4 = x_f0(short, 5);
        xvar<short> x5 = x_f0(short, 4);
        xvar<double> x6 = x_f0(double, 2.5);
        xvar<double> x7 = x_f0(double, 0.5);
        xvar<double> x8 = x_f0(double, -0.5);

        // auto y = x_fn(double, x1 * 2 + x2 + x4, x1, x2);

        auto y = x_fn(double, x1 * x2 + x3*3 - x4*x5*x6 - x7 / x8, x1, x2, x3, x4, x5, x6, x7, x8);
        std::cout<< "y:" << y() << std::endl;
          // y = 1 * 2 + 3 * 3 - 5*4*2.5 + 0.5 / 0.5
        test(y() == -38.0, "x fn for each test 1.1");

        std::cout<< "***********" << std::endl;

        x1.setValue(x1()*2);
        x2.setValue(x2()*2);
        x3.setValue(x3()*2);
        x4.setValue(x4()*2);
        x5.setValue(-x5());
        x6.setValue(x6()*2);
        x7.setValue(x7()*3);
        x8.setValue(x8()*2);
        std::cout<< "y:" << y() << std::endl;

        test(y() == 227.5, "x fn for each test 1.2");
    }

}

#define LOG(fmt, ...) printf("[" fmt "]" __VA_OPT__(,) __VA_ARGS__)

int main() {
    test_x_f0();
    test_x_f1();
    test_x_f2();
    test_x_f2_auto();
    test_x_operator_add();
    test_x_operator_unm();
    test_x_const_lift();
    test_x_fn();
    test_x_exp();
    test_x_fn_foreach();
    // LOG("hello");         // => printf("[" "hello" "]" );]"

    return 0;
}

