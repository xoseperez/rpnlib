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

void test_math(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "5 2 * 3 + 5 mod"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(3, value);

}

void test_math_advanced(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "10 2 pow sqrt log10"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);

}

void test_trig(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "pi 4 / cos 2 sqrt *"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);

}

void test_cast(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "pi 2 round pi 4 round 1.1 floor 1.1 ceil"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(4, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(2, value);
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(3.1416, value);
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(3.14, value);

}

void test_conditional(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "1 2 3 ifn"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(2, value);

}

void test_stack(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "1 3 dup unrot swap - *"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(6, value);

}

void test_logic(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "1 1 == 1 1 != 2 1 > 2 1 <"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(4, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 0, value));
    TEST_ASSERT_EQUAL_FLOAT(0, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 1, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 2, value));
    TEST_ASSERT_EQUAL_FLOAT(0, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 3, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);

}

void test_boolean(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "2 0 and 2 0 or 2 0 xor 1 not"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);
    TEST_ASSERT_EQUAL(4, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 0, value));
    TEST_ASSERT_EQUAL_FLOAT(0, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 1, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 2, value));
    TEST_ASSERT_EQUAL_FLOAT(1, value);
    TEST_ASSERT_TRUE(rpn_stack_get(ctxt, 3, value));
    TEST_ASSERT_EQUAL_FLOAT(0, value);

}

void test_variable(void) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_variable_set(ctxt, "tmp", 25));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "$tmp 5 /"));
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(5, value);
    TEST_ASSERT_EQUAL(1, rpn_variables_size(ctxt));
    TEST_ASSERT_TRUE(rpn_variables_clear(ctxt));
    TEST_ASSERT_EQUAL(0, rpn_variables_size(ctxt));

}

void test_custom_function(void) {

    float value;
    rpn_context ctxt;
    
    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_function_set(ctxt, "cube", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        rpn_stack_push(ctxt, a*a*a);
        return true;
    }));
    TEST_ASSERT_TRUE(rpn_process(ctxt, "3 cube"));
    TEST_ASSERT_EQUAL(1, rpn_stack_size(ctxt));
    TEST_ASSERT_TRUE(rpn_stack_pop(ctxt, value));
    TEST_ASSERT_EQUAL_FLOAT(27, value);

}

void test_error_divide_by_zero(void) {

    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_FALSE(rpn_process(ctxt, "5 0 /"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_DIVIDE_BY_ZERO, rpn_error);

}

void test_error_argument_count_mismatch(void) {

    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_FALSE(rpn_process(ctxt, "1 +"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_ARGUMENT_COUNT_MISMATCH, rpn_error);

}

void test_error_unknown_token(void) {

    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_FALSE(rpn_process(ctxt, "1 2 sum"));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_UNKNOWN_TOKEN, rpn_error);

}

void test_memory(void) {

    unsigned long start = ESP.getFreeHeap();

    {
        rpn_context ctxt;
        TEST_ASSERT_TRUE(rpn_init(ctxt));
        TEST_ASSERT_TRUE(rpn_variable_set(ctxt, "value", 5));
        TEST_ASSERT_TRUE(rpn_process(ctxt, "$value dup 1 - dup 1 - dup 1 - dup 1 -"));
        TEST_ASSERT_TRUE(rpn_clear(ctxt));
    }

    TEST_ASSERT_EQUAL_INT32(start, ESP.getFreeHeap());

}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_math);
    RUN_TEST(test_math_advanced);
    RUN_TEST(test_trig);
    RUN_TEST(test_cast);
    RUN_TEST(test_conditional);
    RUN_TEST(test_stack);
    RUN_TEST(test_logic);
    RUN_TEST(test_boolean);
    RUN_TEST(test_variable);
    RUN_TEST(test_custom_function);
    RUN_TEST(test_error_divide_by_zero);
    RUN_TEST(test_error_argument_count_mismatch);
    RUN_TEST(test_error_unknown_token);
    RUN_TEST(test_memory);
    UNITY_END();
}

void loop() {
    delay(1);
}