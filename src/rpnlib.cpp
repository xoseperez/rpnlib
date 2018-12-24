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
#include "rpnlib_functions.cpp"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

rpn_errors rpn_error = RPN_ERROR_OK;

// ----------------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------------

bool _rpn_is_number(const char * s) {
    unsigned char len = strlen(s);
    if (0 == len) return false;
    bool decimal = false;
    bool digit = false;
    for (unsigned char i=0; i<len; i++) {
        if (('-' == s[i]) || ('+' == s[i])) {
            if (i>0) return false;
        } else if (s[i] == '.') {
            if (!digit) return false;
            if (decimal) return false;
            decimal = true;
        } else if (!isdigit(s[i])) {
            return false;
        } else {
            digit = true;
        }
    }
    return digit;
}

// ----------------------------------------------------------------------------
// Stack methods
// ----------------------------------------------------------------------------

unsigned char rpn_stack_size(rpn_context & ctxt) {
    return ctxt.stack.size();
}

bool rpn_stack_clear(rpn_context & ctxt) {
    ctxt.stack.clear();
    return true;
}

bool rpn_stack_push(rpn_context & ctxt, float value) {
    ctxt.stack.push_back(value);
    return true;
}

bool rpn_stack_pop(rpn_context & ctxt, float & value) {
    if (0 == ctxt.stack.size()) return false;
    value = ctxt.stack.back();
    ctxt.stack.pop_back();
    return true;
}

// ----------------------------------------------------------------------------
// Functions methods
// ----------------------------------------------------------------------------

bool rpn_function_set(rpn_context & ctxt, const char * name, unsigned char argc, bool (*callback)(rpn_context &)) {
    rpn_function f;
    f.name = strdup(name);
    f.argc = argc;
    f.callback = callback;
    ctxt.functions.push_back(f);
    return true;
}

// ----------------------------------------------------------------------------
// Variables methods
// ----------------------------------------------------------------------------

bool rpn_variable_set(rpn_context & ctxt, const char * name, float value) {
    rpn_variable v;
    v.name = strdup(name);
    v.value = value;
    ctxt.variables.push_back(v);
    return true;
}

bool rpn_variable_get(rpn_context & ctxt, const char * name, float & value) {
    for (auto & v : ctxt.variables) {
        if (strcmp(v.name, name) == 0) {
            value = v.value;
            return true;
        }
    }
    return true;
}

bool rpn_variable_del(rpn_context & ctxt, const char * name) {
    for (auto v = ctxt.variables.begin(); v != ctxt.variables.end(); v++) {
        if (strcmp((*v).name, name) == 0) {
            free((*v).name);
            ctxt.variables.erase(v);
            return true;
        }
    }
    return true;
}

bool rpn_variables_clear(rpn_context & ctxt) {
    for (auto & v : ctxt.variables) {
        free(v.name);
    }
    ctxt.variables.clear();
    return true;
}

// ----------------------------------------------------------------------------
// Main methods
// ----------------------------------------------------------------------------

bool rpn_process(rpn_context & ctxt, const char * input) {

    char * token;
    char * base = strdup(input);

    for (token = strtok(base, " "); token != NULL; token = strtok(NULL, " ")) {
        
        // Multiple spaces
        if (0 == strlen(token)) continue;

        // Is token a number?
        if (_rpn_is_number(token)) {
            ctxt.stack.push_back(atof(token));
            continue;
        }

        // Is token a function?
        {
            bool found = false;
            for (auto & f : ctxt.functions) {
                if (strcmp(f.name, token) == 0) {
                    if (rpn_stack_size(ctxt) < f.argc) {
                        rpn_error = RPN_ERROR_ARGUMENT_COUNT_MISMATCH;
                        return false;
                    }
                    if (!(f.callback)(ctxt)) {
                        // Method should set rpn_error
                        return false;
                    }
                    found = true;
                    break;
                }
            }
            if (found) continue;
        }

        // Is token a variable?
        {
            float value;
            if (rpn_variable_get(ctxt, token, value)) {
                ctxt.stack.push_back(value);
                continue;
            }
        }

        // Don't know the token
        rpn_error = RPN_ERROR_UNKNOWN_TOKEN;
        return false;

    }
    
    rpn_error = RPN_ERROR_OK;
    return true;

}

bool rpn_begin(rpn_context & ctxt) {

    rpn_function_set(ctxt, "+", 2, _rpn_sum);
    rpn_function_set(ctxt, "-", 2, _rpn_substract);
    rpn_function_set(ctxt, "*", 2, _rpn_times);
    rpn_function_set(ctxt, "/", 2, _rpn_divide);

    rpn_function_set(ctxt, "==", 2, _rpn_eq);
    rpn_function_set(ctxt, "!=", 2, _rpn_ne);
    rpn_function_set(ctxt, ">", 2, _rpn_gt);
    rpn_function_set(ctxt, ">=", 2, _rpn_ge);
    rpn_function_set(ctxt, "<", 2, _rpn_lt);
    rpn_function_set(ctxt, "<=", 2, _rpn_le);

    rpn_function_set(ctxt, "and", 2, _rpn_and);
    rpn_function_set(ctxt, "or", 2, _rpn_or);
    rpn_function_set(ctxt, "xor", 2, _rpn_xor);
    rpn_function_set(ctxt, "not", 1, _rpn_not);

    rpn_function_set(ctxt, "dup", 1, _rpn_dup);
    rpn_function_set(ctxt, "swap", 2, _rpn_swap);
    rpn_function_set(ctxt, "rot", 3, _rpn_rot);
    rpn_function_set(ctxt, "size", 0, _rpn_size);

    return true;
}

