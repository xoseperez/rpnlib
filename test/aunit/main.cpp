/*

RPNlib

AUnit Unit Tests

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
#include <rpnlib.h>
#include <AUnit.h>

using namespace aunit;

// -----------------------------------------------------------------------------
// Test class
// -----------------------------------------------------------------------------

class CustomTest: public TestOnce {

    protected:

        virtual void setup() override {
            assertTrue(rpn_init(ctxt));
        }

        virtual void teardown() override {
            assertTrue(rpn_clear(ctxt));
        }

        virtual void compare(unsigned char depth, float * expected) {
            assertEqual(RPN_ERROR_OK, rpn_error);
            assertEqual(depth, rpn_stack_size(ctxt));
            float value;
            for (unsigned char i=0; i<depth; i++) {
                assertTrue(rpn_stack_get(ctxt, i, value));
                assertNear(expected[i], value, 0.000001);
            }
        }

        virtual void run_and_compare(const char * command, unsigned char depth, float * expected) {
            assertTrue(rpn_process(ctxt, command));
            compare(depth, expected);
        }

        virtual void run_and_error(const char * command, unsigned char error_code) {
            assertFalse(rpn_process(ctxt, command));
            assertEqual(error_code, rpn_error);
        }

        rpn_context ctxt;

};

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

testF(CustomTest, test_math) {
    float expected[] = {3};
    run_and_compare("5 2 * 3 + 5 mod", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_math_advanced) {
    float expected[] = {1};
    run_and_compare("10 2 pow sqrt log10", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_trig) {
    float expected[] = {1};
    run_and_compare("pi 4 / cos 2 sqrt *", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_cast) {
    float expected[] = {2, 1, 3.1416, 3.14};
    run_and_compare("pi 2 round pi 4 round 1.1 floor 1.1 ceil", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_map) {
    float expected[] = {25};
    run_and_compare("256 0 1024 0 100 map", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_index) {
    float expected[] = {30};
    run_and_compare("2 10 20 30 40 50 5 index", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_cmp3_below) {
    float expected[] = {-1};
    run_and_compare("13 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_cmp3_between) {
    float expected[] = {0};
    run_and_compare("18 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_cmp3_above) {
    float expected[] = {1};
    run_and_compare("25 18 24 cmp3", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_conditional) {
    float expected[] = {2};
    run_and_compare("1 2 3 ifn", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_stack) {
    float expected[] = {6};
    run_and_compare("1 3 dup unrot swap - *", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_logic) {
    float expected[] = {0, 1, 0, 1};
    run_and_compare("1 1 eq 1 1 ne 2 1 gt 2 1 lt", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_boolean) {
    float expected[] = {0, 1, 1, 0};
    run_and_compare("2 0 and 2 0 or 2 0 xor 1 not", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_variable) {
    assertTrue(rpn_variable_set(ctxt, "tmp", 25));
    float expected[] = {5};
    run_and_compare("$tmp 5 /", sizeof(expected)/sizeof(float), expected);
    assertEqual(1, rpn_variables_size(ctxt));
    assertTrue(rpn_variables_clear(ctxt));
    assertEqual(0, rpn_variables_size(ctxt));
}

testF(CustomTest, test_custom_operator) {
    assertTrue(rpn_operator_set(ctxt, "cube", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        rpn_stack_push(ctxt, a*a*a);
        return true;
    }));
    float expected[] = {27};
    run_and_compare("3 cube", sizeof(expected)/sizeof(float), expected);
}

testF(CustomTest, test_error_divide_by_zero) {
    run_and_error("5 0 /", RPN_ERROR_DIVIDE_BY_ZERO);
}

testF(CustomTest, test_error_argument_count_mismatch) {
    run_and_error("1 +", RPN_ERROR_ARGUMENT_COUNT_MISMATCH);
}

testF(CustomTest, test_error_unknown_token) {
    run_and_error("1 2 sum", RPN_ERROR_UNKNOWN_TOKEN);
}

test(test_memory) {
    
    unsigned long start = ESP.getFreeHeap();

    {
        rpn_context ctxt;
        assertTrue(rpn_init(ctxt));
        assertTrue(rpn_variable_set(ctxt, "value", 5));
        assertTrue(rpn_process(ctxt, "$value dup 1 - dup 1 - dup 1 - dup 1 -"));
        assertTrue(rpn_clear(ctxt));
    }

    assertEqual(start, (unsigned long) ESP.getFreeHeap());

}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

void setup() {

    Serial.begin(115200);
    delay(2000);

    Printer::setPrinter(&Serial);
    //TestRunner::setVerbosity(Verbosity::kAll);

}

void loop() {
    TestRunner::run();
    delay(1);
}
