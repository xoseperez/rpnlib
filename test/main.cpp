/*

RPNlib

PlatformIO Unit Tests

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

#include <Arduino.h>
#include "rpnlib.h"
#include <unity.h>

void test_basic(void) {

    rpn_context ctxt;
    rpn_begin(ctxt);
    rpn_process(ctxt, "5 dup *");

    float value;

    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    rpn_stack_pop(ctxt, value);
    TEST_ASSERT_EQUAL(25, value);

}

void test_variable(void) {

    rpn_context ctxt;
    rpn_begin(ctxt);
    rpn_variable_set(ctxt, "tmp", 25);
    rpn_process(ctxt, "tmp 5 /");

    float value;

    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    rpn_stack_pop(ctxt, value);
    TEST_ASSERT_EQUAL(5, value);

}

void test_custom_function(void) {

    rpn_context ctxt;
    rpn_begin(ctxt);
    rpn_function_set(ctxt, "cube", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        rpn_stack_push(ctxt, a*a*a);
        return true;
    });
    rpn_process(ctxt, "3 cube");

    float value;

    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    rpn_stack_pop(ctxt, value);
    TEST_ASSERT_EQUAL(27, value);

}


void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_basic);
    RUN_TEST(test_variable);
    RUN_TEST(test_custom_function);
    UNITY_END();
}

void loop() {
    delay(1);
}