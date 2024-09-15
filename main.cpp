#include "xvar.h"
#include <iostream>
#include <assert.h>

int main() {
    xvar<int> x = x_const(int, 3);
    std::cout<< "x:" << x() << std::endl;
    assert(x() == 3);

    std::cout<< "===========" << std::endl;
    xvar<int> w = x_f1(int, x, x*x);
    std::cout<< "w:" << w() << std::endl;
    assert(w() == 9);

    x.setValue(4);

    std::cout<< "w:" << w() << std::endl;
    assert(w() == 16);

    return 0;
}

