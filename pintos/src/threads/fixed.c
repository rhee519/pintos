#include "threads/fixed.h"

/* Arithmetic operations for FIXED_T. */
fixed_t int_to_fixed(int i)
{
    return i << B_INT;
}

int fixed_to_int(fixed_t f)
{
    return f >> B_INT;
}

fixed_t f_add(fixed_t f1, fixed_t f2)
{
    return f1 + f2;
}

fixed_t f_sub(fixed_t f1, fixed_t f2)
{
    return f1 - f2;
}

fixed_t f_mul(fixed_t f1, fixed_t f2)
{
    int64_t tmp = f1 * f2;
    return tmp >> B_INT;
}

fixed_t f_div(fixed_t f1, fixed_t f2)
{
    return f1 / f2;
}

fixed_t f_add_i(fixed_t f, int i)
{
    return f + (i << B_INT);
}

fixed_t f_mul_i(fixed_t f, int i)
{
    int64_t tmp = f * int_to_fixed(i);
    return tmp >> B_INT;
}

fixed_t f_div_i(fixed_t f, int i)
{
    return (f / i) >> B_INT;
}
