#ifndef MYPROJECTNAME_DATARACE_H
#define MYPROJECTNAME_DATARACE_H

#include <juce_gui_extra/juce_gui_extra.h>

using namespace juce;

struct Data
{
    uint64 a = 0;
    uint64 b = 0;

    bool operator==(const Data& other) const;
    bool operator!=(const Data& other) const;
};

const Data a = {0xaaaaaaaa, 0xaaaaaaaa};
const Data b = {0xbbbbbbbb, 0xbbbbbbbb};

struct A : Thread
{
    explicit A(Data& d) : Thread("A"), data{d} {startThread(); }
    ~A() override { stopThread(100); }
    void run() override;
    Data& data;
};

struct B : Thread
{
    explicit B(Data& d) : Thread("B"), data{d} { startThread(); }
    ~B() override { stopThread(100); }
    void run() override;
    Data& data;
};

struct Test
{
    Data d;
    A a{d};
    B b{d};
};

struct LockingStruct {
    void threadAFunc();
    void threadBFunc();

    int datamember{};
    CriticalSection criticalSection;
};

#endif //MYPROJECTNAME_DATARACE_H
