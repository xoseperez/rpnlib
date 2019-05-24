/*

RPNlib

Copyright (C) 2018-2019 by Xose Pérez <xose dot perez at gmail dot com>

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
#include "rpnlib_operators.cpp"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

rpn_errors rpn_error = RPN_ERROR_OK;
void(*_rpn_debug_callback)(rpn_context &, char *) = NULL;

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

bool rpn_stack_get(rpn_context & ctxt, unsigned char index, float & value) {
    unsigned char size = ctxt.stack.size();
    if (index >= size) return false;
    value = ctxt.stack[size-index-1];
    return true;
}

// ----------------------------------------------------------------------------
// Functions methods
// ----------------------------------------------------------------------------

bool rpn_operator_set(rpn_context & ctxt, const char * name, unsigned char argc, bool (*callback)(rpn_context &)) {
    rpn_operator f;
    f.name = strdup(name);
    f.argc = argc;
    f.callback = callback;
    ctxt.operators.push_back(f);
    return true;
}

bool rpn_operators_clear(rpn_context & ctxt) {
    for (auto & v : ctxt.operators) {
        free(v.name);
    }
    ctxt.operators.clear();
    return true;
}

bool rpn_operators_init(rpn_context & ctxt) {

    rpn_operator_set(ctxt, "pi", 0, _rpn_pi);
    rpn_operator_set(ctxt, "e", 0, _rpn_e);

    rpn_operator_set(ctxt, "+", 2, _rpn_sum);
    rpn_operator_set(ctxt, "-", 2, _rpn_substract);
    rpn_operator_set(ctxt, "*", 2, _rpn_times);
    rpn_operator_set(ctxt, "/", 2, _rpn_divide);
    rpn_operator_set(ctxt, "mod", 2, _rpn_mod);

    rpn_operator_set(ctxt, "round", 2, _rpn_round);
    rpn_operator_set(ctxt, "ceil", 1, _rpn_ceil);
    rpn_operator_set(ctxt, "floor", 1, _rpn_floor);
    rpn_operator_set(ctxt, "int", 1, _rpn_floor);

    #ifdef RPNLIB_ADVANCED_MATH
    rpn_operator_set(ctxt, "sqrt", 1, _rpn_sqrt);
    rpn_operator_set(ctxt, "log", 1, _rpn_log);
    rpn_operator_set(ctxt, "log10", 1, _rpn_log10);
    rpn_operator_set(ctxt, "exp", 1, _rpn_exp);
    rpn_operator_set(ctxt, "fmod", 2, _rpn_fmod);
    rpn_operator_set(ctxt, "pow", 2, _rpn_pow);
    rpn_operator_set(ctxt, "cos", 1, _rpn_cos);
    rpn_operator_set(ctxt, "sin", 1, _rpn_sin);
    rpn_operator_set(ctxt, "tan", 1, _rpn_tan);
    #endif

    rpn_operator_set(ctxt, "==", 2, _rpn_eq);
    rpn_operator_set(ctxt, "!=", 2, _rpn_ne);
    rpn_operator_set(ctxt, ">", 2, _rpn_gt);
    rpn_operator_set(ctxt, ">=", 2, _rpn_ge);
    rpn_operator_set(ctxt, "<", 2, _rpn_lt);
    rpn_operator_set(ctxt, "<=", 2, _rpn_le);

    rpn_operator_set(ctxt, "cmp", 2, _rpn_cmp);
    rpn_operator_set(ctxt, "cmp3", 3, _rpn_cmp3);
    rpn_operator_set(ctxt, "index", 1, _rpn_index);
    rpn_operator_set(ctxt, "map", 5, _rpn_map);
    rpn_operator_set(ctxt, "constrain", 3, _rpn_constrain);

    rpn_operator_set(ctxt, "and", 2, _rpn_and);
    rpn_operator_set(ctxt, "or", 2, _rpn_or);
    rpn_operator_set(ctxt, "xor", 2, _rpn_xor);
    rpn_operator_set(ctxt, "not", 1, _rpn_not);

    rpn_operator_set(ctxt, "dup", 1, _rpn_dup);
    rpn_operator_set(ctxt, "dup2", 2, _rpn_dup2);
    rpn_operator_set(ctxt, "swap", 2, _rpn_swap);
    rpn_operator_set(ctxt, "rot", 3, _rpn_rot);
    rpn_operator_set(ctxt, "unrot", 3, _rpn_unrot);
    rpn_operator_set(ctxt, "drop", 1, _rpn_drop);
    rpn_operator_set(ctxt, "over", 2, _rpn_over);
    rpn_operator_set(ctxt, "depth", 0, _rpn_depth);

    rpn_operator_set(ctxt, "ifn", 3, _rpn_ifn);
    rpn_operator_set(ctxt, "end", 1, _rpn_end);

    return true;
}

// ----------------------------------------------------------------------------
// Variables methods
// ----------------------------------------------------------------------------

bool rpn_variable_set(rpn_context & ctxt, const char * name, float value) {
    for (auto & v : ctxt.variables) {
        if (strcmp(v.name, name) == 0) {
            v.value = value;
            return true;
        }
    }
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
    return false;
}

bool rpn_variable_del(rpn_context & ctxt, const char * name) {
    for (auto v = ctxt.variables.begin(); v != ctxt.variables.end(); v++) {
        if (strcmp((*v).name, name) == 0) {
            free((*v).name);
            ctxt.variables.erase(v);
            return true;
        }
    }
    return false;
}

unsigned char rpn_variables_size(rpn_context & ctxt) {
    return ctxt.variables.size();
}

char * rpn_variable_name(rpn_context & ctxt, unsigned char i) {
    if (i < ctxt.variables.size()) {
        return ctxt.variables[i].name;
    }
    return NULL;
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

bool rpn_process(rpn_context & ctxt, const char * input, bool variable_must_exist) {

    char * token;
    char * base = strdup(input);

    rpn_error = RPN_ERROR_OK;

    for (token = strtok(base, " "); token != NULL; token = strtok(NULL, " ")) {
        
        // Debug callback
        if (_rpn_debug_callback) {
            (*_rpn_debug_callback)(ctxt, token);
        }

        // Multiple spaces
        if (0 == strlen(token)) {
            continue;
        }

        // Is token a number?
        if (_rpn_is_number(token)) {
            ctxt.stack.push_back(atof(token));
            continue;
        }

        // Is token a operator?
        {
            bool found = false;
            for (auto & f : ctxt.operators) {
                if (strcmp(f.name, token) == 0) {
                    if (rpn_stack_size(ctxt) < f.argc) {
                        rpn_error = RPN_ERROR_ARGUMENT_COUNT_MISMATCH;
                        break;
                    }
                    if (!(f.callback)(ctxt)) {
                        // Method should set rpn_error
                        break;
                    }
                    found = true;
                    break;
                }
            }
            if (RPN_ERROR_OK != rpn_error) break;
            if (found) continue;
        }

        // Is token a variable?
        {
            if (token[0] == '$') {
                float value = 0;
                bool exists = rpn_variable_get(ctxt, &token[1], value);
                if (exists || !variable_must_exist) {
                ctxt.stack.push_back(value);
                continue;
                }
            }
        }

        // Don't know the token
        rpn_error = RPN_ERROR_UNKNOWN_TOKEN;
        break;

    }
    
    free(base);
    return (RPN_ERROR_OK == rpn_error);

}

bool rpn_debug(void(*callback)(rpn_context &, char *)) {
    _rpn_debug_callback = callback;
    return true;
}

bool rpn_init(rpn_context & ctxt) {
    return rpn_operators_init(ctxt);
}

bool rpn_clear(rpn_context & ctxt) {
    rpn_operators_clear(ctxt);
    rpn_variables_clear(ctxt);
    rpn_stack_clear(ctxt);
    return true;
}