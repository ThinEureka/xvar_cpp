#include "xvar.h"
#include <iostream>
#include <assert.h>
#include <string>

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

void test_x_fn() {
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
}

int main() {
    test_x_f0();
    test_x_f1();
    test_x_f2();
    test_x_f2_auto();
    test_x_operator_add();
    test_x_operator_unm();
    test_x_const_lift();
    test_x_fn();

    return 0;
}

