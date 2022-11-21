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
    return (fixed_t)(((int64_t)f1 * f2) >> B_INT);
}

fixed_t f_div(fixed_t f1, fixed_t f2)
{
    int shift = 32;
    return (fixed_t)((((int64_t)f1 << shift) / f2) >> (shift - B_INT));
}