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

#define COMMAND     "5 dup *"

void setup() {
    
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println();
    
    rpn_context ctxt;
    rpn_begin(ctxt);
    rpn_process(ctxt, COMMAND);
    
    unsigned char size = rpn_stack_size(ctxt);
    Serial.printf("Stack size: %d\n", size);
    
    float value;
    for (unsigned char i=0; i<size; i++) {
        rpn_stack_pop(ctxt, value);
        Serial.printf("Stack level #%d value: %f\n", i, value);
    }

}

void loop() {

}