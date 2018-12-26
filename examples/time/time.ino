/*

RPNlib

Basic example

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
#include <Time.h>

void dump_stack(rpn_context & ctxt) {
    float value;
    unsigned char index = rpn_stack_size(ctxt)-1;
    Serial.printf("Stack\n--------------------\n");
    while (rpn_stack_get(ctxt, index, value)) {
        Serial.printf("[%02d] %.2f\n", index--, value);
    }
    Serial.println();
}

void setup() {
    
    // Init serial communication with the computer
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println();
    
    // Initialize time
    // This would normally be set via NTP or GPS
    setTime(17, 8, 24, 26, 12, 2018);

    // Create context
    rpn_context ctxt;
    
    // Initialize context
    rpn_init(ctxt);

    // Add custom time functions
    rpn_operator_set(ctxt, "now", 0, [](rpn_context & ctxt) {
        rpn_stack_push(ctxt, now());
        return true;
    });
    rpn_operator_set(ctxt, "dow", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        unsigned char dow = (weekday(int(a)) + 5) % 7;
        rpn_stack_push(ctxt, dow);
        return true;
    });
    rpn_operator_set(ctxt, "hour", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        rpn_stack_push(ctxt, hour(int(a)));
        return true;
    });
    rpn_operator_set(ctxt, "minute", 1, [](rpn_context & ctxt) {
        float a;
        rpn_stack_pop(ctxt, a);
        rpn_stack_push(ctxt, minute(int(a)));
        return true;
    });

    // Process command
    rpn_process(ctxt, "now dup dup dow rot hour rot minute");
    
    // Show final stack
    dump_stack(ctxt);

    // Clear the context and free resources
    rpn_clear(ctxt);

}

void loop() {
    delay(1);
}