#include "threads/fixed.h"

/* Convert INT to FIXED_T. */
fixed_t int_to_fixed(int i)
{
    return i << B_INT;
}

/* Round FIXED_T to INT. */
int fixed_to_int(fixed_t f)
{
    int sign = f & F_SIGN ? -1 : 1;
    f += sign * (F_INT >> 1);
    return f >> B_INT;
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