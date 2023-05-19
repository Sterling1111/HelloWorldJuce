#ifndef _65C02_SYSTEM_EMULATION_CYCLES_H
#define _65C02_SYSTEM_EMULATION_CYCLES_H
#include "system_types.h"


class Cycles {
public:
    Cycles();
    Cycles& operator++();
    Cycles& operator+=(sdword);
    void reset();
    [[nodiscard]] uint64_t getCycles() const;

private:
    uint64_t cycles{};
};



#endif //_65C02_SYSTEM_EMULATION_CYCLES_H
