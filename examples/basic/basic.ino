/*

RPNlib

Basic example

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

    // Process command (does 5 times 5)
    rpn_process(ctxt, "5 dup *");
    
    // Show final stack
    dump_stack(ctxt);

    // Clear the context and free resources
    rpn_clear(ctxt);

}

void loop() {
    delay(1);
}