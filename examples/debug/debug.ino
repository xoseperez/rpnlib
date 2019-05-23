/*

RPNlib

Debug example

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

void dump_stack(rpn_context & ctxt) {
    float value;
    unsigned char index = rpn_stack_size(ctxt)-1;
    Serial.printf("Stack\n--------------------\n");
    while (rpn_stack_get(ctxt, index, value)) {
        Serial.printf("[%02d] %.2f\n", index--, value);
    }
    Serial.println();
}

void dump_variables(rpn_context & ctxt) {
    float value;
    char * name;
    unsigned char index = 0;
    Serial.printf("Variables\n--------------------\n");
    while ((name = rpn_variable_name(ctxt, index))) {
        rpn_variable_get(ctxt, name, value);
        Serial.printf("%s = %.2f\n", name, value);
        index++;
    }
    Serial.println();
}

void setup() {
    
    // Init serial communication with the computer
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println();
    
    // Create context
    rpn_context ctxt;
    
    // Initialize context
    rpn_init(ctxt);

    // Define debug callback
    // The callback returns the current context and
    // the token that is going to be processed next
    rpn_debug([](rpn_context & ctxt, char * token) {
        dump_stack(ctxt);
        Serial.printf("Processing: %s\n\n", token);
    });

    // Load variables
    rpn_variable_set(ctxt, "temperature", 22);
    rpn_variable_set(ctxt, "relay", 1);

    // Show variables
    dump_variables(ctxt);

    // Process command
    // This command checks if the temperature is below 18 or above 21 degrees and 
    // returns the expected relay (controlling a heater) accordingly. 
    // If the temperature is in between those values the relay status does not change.
    // This is a simple hysteresis behaviour.
    rpn_process(ctxt, "$temperature dup 18 < swap 21 < dup2 xor not unrot and $relay ifn");
    
    // Show final stack
    dump_stack(ctxt);

    // Show result
    if (rpn_stack_size(ctxt) == 1) {
        float value;
        rpn_stack_pop(ctxt, value);
        Serial.printf("Relay status should be: %d\n", (int) value);
    } else {
        Serial.println("Stack should have only 1 value");
    }

    // Clear the context and free resources
    rpn_clear(ctxt);

}

void loop() {
    delay(1);
}