#include "includes.h"

IRReader::IRReader(){}; 
IRReader::IRReader(int pin1k, int pin10k) { 
    this->pin1k = pin1k;
    this->pin10k = pin10k; 
}

void IRReader::begin() { 
    pinMode(pin1k, INPUT_PULLDOWN); 
    pinMode(pin10k, INPUT_PULLDOWN); 
}

bool IRReader::read1k() { 
    return digitalRead(pin1k); 
}

bool IRReader::read10k() { 
    return digitalRead(pin10k); 
}