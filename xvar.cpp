
/*!
 * \file AsysValue.h
 * \date 9-24-2024 
 * 
 * https://github.com/ThinEureka/xvar_cpp
 * \author cs (04nycs@gmail.com)
 */

#define XVAR_TEST_LIFETIME 
#include "xvar.h"

#ifdef XVAR_TEST_LIFETIME 
    int xvar_base::s_totalCount = 0;
#endif
