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

        xvar<int> y = x_f1(int, x, x*x);
        std::cout<< "y:" << y() << std::endl;
        test(y() == 9, "f1 test 1.2.1");
        x.setValue(4);

        std::cout<< "***********" << std::endl;

        std::cout<< "y:" << y() << std::endl;
        test(y() == 16, "f1 test 1.2.2");
    }

    std::cout<< "===========" << std::endl;

    {
        xvar<int> x = x_f0(int, 3);
        std::cout<< "x:" << x() << std::endl;
        test(x() == 3, "f1 test 2.1");

        xvar<std::string> y = x_f1(std::string, x, std::to_string(x) + "_kkk");
        std::cout<< "y:" << y() << std::endl;
        test(y() == "3_kkk", "f1 test 2.1.1");

        std::cout<< "***********" << std::endl;
        x.setValue(4);

        std::cout<< "y:" << y() << std::endl;
        test(y() == "4_kkk", "f1 test 2.2.2");
    }
}

int main() {
    test_x_f0();
    test_x_f1();

    // xvar<int> w = x_f1(int, x, x*x);
    // std::cout<< "w:" << w() << std::endl;
    // assert(w() == 9);
    // x.setValue(4);

    return 0;
}

