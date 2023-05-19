#include <iostream>
#include "Cycles.h"

Cycles::Cycles() = default;

Cycles& Cycles::operator++() {
    ++cycles;
    return *this;
}

Cycles& Cycles::operator+=(sdword num) {
    for (int i = 0; i < num; ++i) {
        this->operator++();
    }
    return *this;
}

void Cycles::reset() {
    cycles = 0;
}

uint64_t Cycles::getCycles() const {return cycles;}