/*

RPNlib

AUnit Unit Tests

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
    while (!Serial);

    Printer::setPrinter(&Serial);
    //TestRunner::setVerbosity(Verbosity::kAll);

}

void loop() {
    TestRunner::run();
    delay(1);
}
