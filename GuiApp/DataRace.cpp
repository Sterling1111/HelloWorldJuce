#include "DataRace.h"

bool Data::operator!=(const Data &other) const {
    return !(this->operator==(other));
}

bool Data::operator==(const Data &other) const {
    return other.a == a && other.b == b;
}

void A::run()
{
    while(true) {
        if(threadShouldExit()) {
            break;
        }
        data = a;

        if(data != a && data != b) {
            auto _local = data;
            DBG( "B: "
                          << "data = { .a=" << String::toHexString(_local.a)
                          << ", .b=" << String::toHexString(_local.b) << " }");
        }

        wait(10);
    }
}

void B::run() {
    while(true) {
        if(threadShouldExit()) {
            break;
        }
        data = b;

        if(data != a && data != b) {
            auto _local = data;
            DBG( "B: "
                      << "data = { .a=" << String::toHexString(_local.a)
                      << ", .b=" << String::toHexString(_local.b) << " }");
        }
        wait(10);
    }
}

void LockingStruct::threadAFunc() {
    {
        const ScopedLock sl(criticalSection);
        datamember = 2;
    }
    DBG("thread A Func: " << datamember << "\n");
}

void LockingStruct::threadBFunc() {
    {
        const ScopedLock sl(criticalSection);
        datamember = 5;
    }
    DBG("thread B Func: " << datamember << "\n");
}
