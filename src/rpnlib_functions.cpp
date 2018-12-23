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

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

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

bool _rpn_swap(rpn_context & ctxt) {
    float a, b;
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, b);
    rpn_stack_push(ctxt, a);
    return true;
}

bool _rpn_rot(rpn_context & ctxt) {
    float a, b, c;
    rpn_stack_pop(ctxt, c);
    rpn_stack_pop(ctxt, b);
    rpn_stack_pop(ctxt, a);
    rpn_stack_push(ctxt, b);
    rpn_stack_push(ctxt, a);
    rpn_stack_push(ctxt, c);
    return true;
}

