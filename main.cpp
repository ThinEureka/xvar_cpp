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
        test(z() == 9, "operator_add test 1.2.1");
    }

    std::cout<< "===========" << std::endl;
}

int main() {
    test_x_f0();
    test_x_f1();
    test_x_f2();
    test_x_f2_auto();
    test_x_operator_add();

    return 0;
}

