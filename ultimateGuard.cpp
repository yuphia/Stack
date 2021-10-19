#include "ultimateGuard.h"
#include "stackFunctions.h"

size_t dumpCounter()
{
    static size_t dumpCounter = 0;

    dumpCounter++;
    return dumpCounter;
}





