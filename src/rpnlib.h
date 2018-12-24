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

#ifndef rpnlib_h
#define rpnlib_h

// ----------------------------------------------------------------------------

#include <vector>

// ----------------------------------------------------------------------------

struct rpn_variable {
    char * name;
    float value;
};

struct rpn_context;

struct rpn_function {
    char * name;
    unsigned char argc;
    bool (*callback)(rpn_context &);
};

struct rpn_context {
    std::vector<float> stack;
    std::vector<rpn_variable> variables;
    std::vector<rpn_function> functions;
};

enum rpn_errors {
    RPN_ERROR_OK,
    RPN_ERROR_UNKNOWN_TOKEN,
    RPN_ERROR_ARGUMENT_COUNT_MISMATCH,
    RPN_ERROR_DIVIDE_BY_ZERO
};

// ----------------------------------------------------------------------------

extern rpn_errors rpn_error;
extern void(*_rpn_debug_callback)(rpn_context &, char *);

// ----------------------------------------------------------------------------

bool rpn_functions_init(rpn_context &);
bool rpn_function_set(rpn_context &, const char *, unsigned char, bool (*)(rpn_context &));
bool rpn_functions_clear(rpn_context &);

bool rpn_variable_set(rpn_context &, const char *, float);
bool rpn_variable_get(rpn_context &, const char *, float &);
bool rpn_variable_del(rpn_context &, const char *);
unsigned char rpn_variables_size(rpn_context &);
char * rpn_variable_name(rpn_context &, unsigned char);
bool rpn_variables_clear(rpn_context &);

bool rpn_stack_clear(rpn_context &);
bool rpn_stack_push(rpn_context &, float);
bool rpn_stack_pop(rpn_context &, float &);
unsigned char rpn_stack_size(rpn_context &);
bool rpn_stack_get(rpn_context &, unsigned char, float &);

bool rpn_process(rpn_context &, const char *);
bool rpn_init(rpn_context &);

bool rpn_debug(void(*)(rpn_context &, char *));

// ----------------------------------------------------------------------------

#endif // rpnlib_h
