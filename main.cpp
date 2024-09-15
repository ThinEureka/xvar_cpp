#include "xvar.h"
#include <iostream>
#include <assert.h>



int main() {

    X_CONST(int, z, 3);
    assert(z() == 3);

    std::cout<< "z:" << z() << std::endl;
    return 0;
}

