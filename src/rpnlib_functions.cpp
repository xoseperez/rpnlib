/*

RPNlib

Copyright (C) 2018 by Xose Pérez <xose dot perez at gmail dot com>

The rpnlib library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The rpnlib library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the rpnlib library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "rpnlib.h"

extern "C" {
    #include "fs_math.h"
}

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

bool _rpn_pi(rpn_context & ctxt) {
    float pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
    rpn_stack_push(ctxt, pi);
    return true;
}

// ----------------------------------------------------------------------------
// Math
// ----------------------------------------------------------------------------

bool _rpn_sum(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a+b);
    return true;
}

bool _rpn_substract(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a-b);
    return true;
}

bool _rpn_times(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a*b);
    return true;
}

bool _rpn_divide(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    if (0 == b) {
        rpn_error = RPN_ERROR_DIVIDE_BY_ZERO;
        return false;
    }
    rpn_stack_push(ctxt, a/b);
    return true;
}

bool _rpn_mod(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    a = (int) a;
    b = (int) b;
    if (0 == b) {
        rpn_error = RPN_ERROR_DIVIDE_BY_ZERO;
        return false;
    }
    float mod = a - (int) (a / b) * b;
    rpn_stack_push(ctxt, mod);
    return true;
}

// ----------------------------------------------------------------------------
// Advanced math
// ----------------------------------------------------------------------------

bool _rpn_sqrt(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, fs_sqrt(a));
    return true;
}

bool _rpn_log(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    if (0 >= a) {
        rpn_error = RPN_ERROR_UNVALID_ARGUMENT;
        return false;
    }
    rpn_stack_push(ctxt, fs_log(a));
    return true;
}

bool _rpn_log10(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    if (0 >= a) {
        rpn_error = RPN_ERROR_UNVALID_ARGUMENT;
        return false;
    }
    rpn_stack_push(ctxt, fs_log10(a));
    return true;
}

bool _rpn_exp(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, fs_exp(a));
    return true;
}

bool _rpn_fmod(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    if (0 == b) {
        rpn_error = RPN_ERROR_DIVIDE_BY_ZERO;
        return false;
    }
    rpn_stack_push(ctxt, fs_fmod(a, b));
    return true;
}

bool _rpn_pow(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, fs_pow(a, b));
    return true;
}

bool _rpn_cos(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, fs_cos(a));
    return true;
}

bool _rpn_sin(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    float cos = fs_cos(a);
    float sin = fs_sqrt(1 - cos * cos);
    rpn_stack_push(ctxt, sin);
    return true;
}

bool _rpn_tan(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    float cos = fs_cos(a);
    if (0 == cos) {
        rpn_error = RPN_ERROR_UNVALID_ARGUMENT;
        return false;
    }
    float sin = fs_sqrt(1 - cos * cos);
    rpn_stack_push(ctxt, sin / cos);
    return true;
}

// ----------------------------------------------------------------------------
// Logic
// ----------------------------------------------------------------------------

bool _rpn_eq(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a == b ? 1 : 0);
    return true;
}

bool _rpn_ne(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a != b ? 1 : 0);
    return true;
}

bool _rpn_gt(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a > b ? 1 : 0);
    return true;
}

bool _rpn_ge(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a >= b ? 1 : 0);
    return true;
}

bool _rpn_lt(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a < b ? 1 : 0);
    return true;
}

bool _rpn_le(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a <= b ? 1 : 0);
    return true;
}

// ----------------------------------------------------------------------------
// Boolean
// ----------------------------------------------------------------------------

bool _rpn_and(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, ((a!=0) & (b!=0)) ? 1 : 0);
    return true;
}

bool _rpn_or(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, ((a!=0) | (b!=0)) ? 1 : 0);
    return true;
}

bool _rpn_xor(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, ((a!=0) ^ (b!=0)) ? 1 : 0);
    return true;
}

bool _rpn_not(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a == 0 ? 1 : 0);
    return true;
}

// ----------------------------------------------------------------------------
// Casting
// ----------------------------------------------------------------------------

bool _rpn_round(rpn_context & ctxt) {
    
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    
    unsigned char decimals = (int) b;
    unsigned long multiplier = 1;
    for (unsigned char i=0; i<decimals; i++) {
        multiplier *= 10;
    }
    a = (float) (int(a * multiplier + 0.5)) / multiplier;

    rpn_stack_push(ctxt, a);
    return true;

}

bool _rpn_ceil(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, int(a) + (a == int(a) ? 0 : 1));
    return true;
}

bool _rpn_floor(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, int(a));
    return true;
}

// ----------------------------------------------------------------------------
// Conditionals
// ----------------------------------------------------------------------------

bool _rpn_ifn(rpn_context & ctxt) {
    float a, b, c;
    rpn_stack_pop(ctxt, c);
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, (a!=0) ? b : c);
    return true;
}

// ----------------------------------------------------------------------------
// Stack
// ----------------------------------------------------------------------------

bool _rpn_dup(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, a);
    return true;
}

bool _rpn_dup2(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, b);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, b);
    return true;
}

bool _rpn_over(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, b);
    rpn_stack_push(ctxt, a);
    return true;
}

bool _rpn_swap(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, b);
    rpn_stack_push(ctxt, a);
    return true;
}

bool _rpn_unrot(rpn_context & ctxt) {
    float a, b, c;
    rpn_stack_pop(ctxt, c);
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, c);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, b);
    return true;
}

bool _rpn_rot(rpn_context & ctxt) {
    float a, b, c;
    rpn_stack_pop(ctxt, c);
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, b); 
    rpn_stack_push(ctxt, c);
    rpn_stack_push(ctxt, a);
    return true;
}

bool _rpn_drop(rpn_context & ctxt) {
    float a;
    rpn_stack_pop(ctxt, a);
    return true;
}

bool _rpn_depth(rpn_context & ctxt) {
    rpn_stack_push(ctxt, rpn_stack_size(ctxt));
    return true;
}

