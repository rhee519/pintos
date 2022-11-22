/**
 * [PROJECT-3] Jiho Rhee
 */

#ifndef THREADS_FIXED_H
#define THREADS_FIXED_H

#include "stdint.h"

/*
   Structure of 4-bytes fixed-point number:
   +-------+------------+--------------+
   | 1 bit | 17 bits    | 14 bits      |
   | sign  | integer(p) | fraction(q)  | total: 32 bits (4 bytes)
   +-------+------------+--------------+
*/
typedef int64_t fixed_t; /* 4-bytes fixed-point number. */
#define B_SIGN 31
#define B_INT 14
#define B_ROUND 13
#define F_SIGN 1 << B_SIGN   /* Flag of sign-bit of fixed-point. */
#define F_INT 1 << B_INT     /* LSB of integer part of fixed-point. */
#define F_ROUND 1 << B_ROUND /* MSB of fraction part of fixed-point (for integer-rounding).*/

fixed_t int_to_fixed(int i);
int fixed_to_int(fixed_t f);
fixed_t f_mul(fixed_t f1, fixed_t f2);
fixed_t f_div(fixed_t f1, fixed_t f2);

#endif