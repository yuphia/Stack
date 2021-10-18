#include "ultimateGuard.h"

void dumpStk (struct stk* stk)
{
    for (size_t i = 0; i < stk->capacity; i++)
    {
        fprintf (logFileConst,  "buffer[%zu] = %d\n", i, stkBuffer (i));
         
        if (i == stk->nElement - 1)
            fprintf (logFileConst, "\n"
                     "Further elements do not contain stack elements,"
                     "but they lie in the bufferized region"
                     "\n"
                     "\n");
    }    
}

size_t dumpCounter()
{
    static size_t dumpCounter = 0;
    dumpCounter++;
    return dumpCounter;
}

enum stkError validityStk (struct stk* stk)
{
    if (stk == nullptr)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to struct stk equals nullptr\n", 1);
        return NULLPOINTERSTRUCT;
    }

    if (stk->canaryL != canaryL)
    {
        LOGDUMP (logFileConst, stk, 0, "Left canary in struct is dead, Most likely the stack"
                                       " struct elements have been accessed from the outside", 1);
        return CANARYL_DEAD;
    }
    
    if (stk->canaryR != canaryR)
    {
        LOGDUMP (logFileConst, stk, 0, "Right canary in struct is dead, Most likely the stack"
                                       " struct elements have been accessed from the outside", 1);
        return CANARYR_DEAD;
    }
  
    if (stkCanaryBufferL != canaryBufferL)
    {
        LOGDUMP (logFileConst, stk, 0, "Left canary in buffer is dead, Most likely the stack"
                                       " buffer elements have been accessed from the outside", 1);
        return CANARYL_BUFF_STK_DEAD;
    }
    
    if (stkCanaryBufferR != canaryBufferR)
    {
        LOGDUMP (logFileConst, stk, 0, "Right canary in buffer is dead, Most likely the stack"
                                       " buffer elements have been accessed from the outside", 1);
        return CANARYR_BUFF_STK_DEAD;
    }
  
    if (stk->buffer == nullptr && stk->lastError == REALLOCNOMEM)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to buffer equals nullptr. Latest reallocation has failed", 1); 
        return REALLOCNOMEM;
    }
    
    if (stk->buffer == nullptr && stk->capacity != 0)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to buffer equals nullptr. Latest allocation has failed\n", 1);
        return ALLOCNOMEM;
    }

    if (stk->lastError == STKUNDERFLOW)
    {
        LOGDUMP (logFileConst, stk, 0, "Most likely you have called more pops than needed, be careful", 1);
        return STKUNDERFLOW;
    }

    if (stk->capacity == 0 && stk->buffer == nullptr)
    {    
        LOGDUMP (logFileConst, stk, 0, "The capacity of the stack equal to null - which means it has likely been destructed", 1);
        free (stk->buffer);
        return STKDEAD;
    }

    for (size_t i = stk->nElement; i < stk->capacity; i++)
        if (stkBuffer (i) != stk->poison)
        {         
            LOGDUMP (logFileConst, stk, 0, "Poison values have failed - it means your stack has been accessed with non-stack function", 1);
            free (stk->buffer);
            return STKOUTSIDEACCESS;
        }

    if (stk->nElement > stk->capacity)
    {
        LOGDUMP (logFileConst, stk, 0, "You have tried to access memory that is not bufferized (overflow)", 1);
        free (stk->buffer);
        return STKOVERFLOW;
    }

    LOGDUMP (logFileConst, stk, 1, "EVERYTHING IS OK", 0);

    return NOERR; 
}

