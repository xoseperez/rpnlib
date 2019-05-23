/*

RPNlib

PlatformIO Unit Tests

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

#include <Arduino.h>
#include "rpnlib.h"
#include <unity.h>

// -----------------------------------------------------------------------------
// Helper methods
// -----------------------------------------------------------------------------

void run_and_compare(const char * command, unsigned char depth, float * expected) {

    float value;
    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_process(ctxt, command));
    TEST_ASSERT_EQUAL_INT8(RPN_ERROR_OK, rpn_error);

    TEST_ASSERT_EQUAL_INT8(depth, rpn_stack_size(ctxt));
    for (unsigned char i=0; i<depth; i++) {
        TEST_ASSERT_TRUE(rpn_stack_get(ctxt, i, value));
        TEST_ASSERT_EQUAL_FLOAT(expected[i], value);
    }

}

void run_and_error(const char * command, unsigned char error_code) {

    rpn_context ctxt;

    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_FALSE(rpn_process(ctxt, command));
    TEST_ASSERT_EQUAL_INT8(error_code, rpn_error);

}

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

void test_math(void) {
    float expected[] = {3};
    run_and_compare("5 2 * 3 + 5 mod", sizeof(expected)/sizeof(float), expected);
}

void test_math_advanced(void) {
    float expected[] = {1};
    run_and_compare("10 2 pow sqrt log10", sizeof(expected)/sizeof(float), expected);
}

void test_trig(void) {
    float expected[] = {1};
    run_and_compare("pi 4 / cos 2 sqrt *", sizeof(expected)/sizeof(float), expected);
}

void test_cast(void) {
    float expected[] = {2, 1, 3.1416, 3.14};
    run_and_compare("pi 2 round pi 4 round 1.1 floor 1.1 ceil", sizeof(expected)/sizeof(float), expected);
}

void test_map(void) {
    float expected[] = {25};
    run_and_compare("256 0 1024 0 100 map", sizeof(expected)/sizeof(float), expected);
}

void test_index(void) {
    float expected[] = {30};
    run_and_compare("2 10 20 30 40 50 5 index", sizeof(expected)/sizeof(float), expected);
}

void test_cmp3_below(void) {
    float expected[] = {-1};
    run_and_compare("13 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

void test_cmp3_between(void) {
    float expected[] = {0};
    run_and_compare("18 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

void test_cmp3_above(void) {
    float expected[] = {1};
    run_and_compare("25 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

void test_conditional(void) {
    float expected[] = {2};
    run_and_compare("1 2 3 ifn", sizeof(expected)/sizeof(float), expected);
}

void test_stack(void) {
    float expected[] = {6};
    run_and_compare("1 3 dup unrot swap - *", sizeof(expected)/sizeof(float), expected);
}

void test_logic(void) {
    float expected[] = {0, 1, 0, 1};
    run_and_compare("1 1 == 1 1 != 2 1 > 2 1 <", sizeof(expected)/sizeof(float), expected);
}

void test_boolean(void) {
    float expected[] = {0, 1, 1, 0};
    run_and_compare("2 0 and 2 0 or 2 0 xor 1 not", sizeof(expected)/sizeof(float), expected);
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

void test_custom_operator(void) {

    float value;
    rpn_context ctxt;
    
    TEST_ASSERT_TRUE(rpn_init(ctxt));
    TEST_ASSERT_TRUE(rpn_operator_set(ctxt, "cube", 1, [](rpn_context & ctxt) {
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
    run_and_error("5 0 /", RPN_ERROR_DIVIDE_BY_ZERO);
}

void test_error_argument_count_mismatch(void) {
    run_and_error("1 +", RPN_ERROR_ARGUMENT_COUNT_MISMATCH);
}

void test_error_unknown_token(void) {
    run_and_error("1 2 sum", RPN_ERROR_UNKNOWN_TOKEN);
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

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_math);
    RUN_TEST(test_math_advanced);
    RUN_TEST(test_trig);
    RUN_TEST(test_cast);
    RUN_TEST(test_map);
    RUN_TEST(test_index);
    RUN_TEST(test_cmp3_below);
    RUN_TEST(test_cmp3_between);
    RUN_TEST(test_cmp3_above);
    RUN_TEST(test_conditional);
    RUN_TEST(test_stack);
    RUN_TEST(test_logic);
    RUN_TEST(test_boolean);
    RUN_TEST(test_variable);
    RUN_TEST(test_custom_operator);
    RUN_TEST(test_error_divide_by_zero);
    RUN_TEST(test_error_argument_count_mismatch);
    RUN_TEST(test_error_unknown_token);
    RUN_TEST(test_memory);
    UNITY_END();
}

void loop() {
    delay(1);
}