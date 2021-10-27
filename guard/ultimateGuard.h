/** \file
    \brief Header file that defines macro MY_ASSERT, if flags DEBUG_SOFT and NDEBUG are not defined
    MY_ASSERT (statement, message) checks the statement and if it is false prints the message
    Also it prints the line, function and file where the error has occured
    After printing it aborts the programm

    If DEBUG_SOFT flag is defined does the same but doesn't abort the programm

    If NDEBUG flag is defined does nothing**/

#ifndef ULTIMATEGUARD_H

#define ULTIMATEGUARD_H 1

#include <stdio.h>
#include <math.h>
#include "../stackFuncs/stackFunctions.h"

//===========================================================================
template <typename data>
void dumpStk (struct stk<data>* stk, FILE* const logFileConst);

size_t dumpCounter();

template <typename data>
enum stkError validityStk (struct stk<data>* stk, struct dumpInfo* info);

template <typename data>
hash_t hashCalc (struct stk<data>* stk);

template <typename data>
void dumpFunctionInt (struct stk<data>* stk, size_t i, FILE* const logFileConst);

template <typename data>
enum stkError validityStk (struct stk<data>* stk, struct dumpInfo* info)
{
    FILE* const logFileConst = fopen ("log.txt", "a");

    if (stk == nullptr)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to struct stk equals nullptr\n", 1);
        fclose (logFileConst);
        return NULLPOINTERSTRUCT;
    }

    if (stk->buffer == nullptr && stk->lastError == REALLOCNOMEM)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to buffer equals nullptr. Latest reallocation has failed", 1); 
        fclose (logFileConst);
        return REALLOCNOMEM;
    }
    
    if (stk->buffer == nullptr && stk->capacity != 0)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to buffer equals nullptr. Latest allocation has failed\n", 1);
        fclose (logFileConst);
        return ALLOCNOMEM;
    }

    if (stk->hash != hashCalc (stk))
    {
        LOGDUMP (logFileConst, stk, 0, "HASH POMER VIRUBAY", 1);
        fclose (logFileConst);
        return HASH_DEAD;
    }

    if (stk->canaryL != canaryL)
    {
        LOGDUMP (logFileConst, stk, 0, "Left canary in struct is dead, Most likely the stack"
                                       " struct elements have been accessed from the outside", 1);
        fclose (logFileConst);
        return CANARYL_DEAD;
    }
    
    if (stk->canaryR != canaryR)
    {
        LOGDUMP (logFileConst, stk, 0, "Right canary in struct is dead, Most likely the stack"
                                       " struct elements have been accessed from the outside", 1);
        fclose (logFileConst);
        return CANARYR_DEAD;
    }
  
    if (stkCanaryBufferL != canaryBufferL)
    {
        LOGDUMP (logFileConst, stk, 0, "Left canary in buffer is dead, Most likely the stack"
                                       " buffer elements have been accessed from the outside", 1);
        fclose (logFileConst); 
        return CANARYL_BUFF_STK_DEAD;
    }
    
    if (stkCanaryBufferR != canaryBufferR)
    {
        LOGDUMP (logFileConst, stk, 0, "Right canary in buffer is dead, Most likely the stack"
                                       " buffer elements have been accessed from the outside", 1);
        fclose (logFileConst);
        return CANARYR_BUFF_STK_DEAD;
    }
  
    if (stk->lastError == STKUNDERFLOW)
    {
        LOGDUMP (logFileConst, stk, 0, "Most likely you have called more pops than needed, be careful", 1);
        fclose (logFileConst);
        return STKUNDERFLOW;
    }

    if (stk->capacity == 0 && stk->buffer == nullptr)
    {    
        LOGDUMP (logFileConst, stk, 0, "The capacity of the stack equal to null - which means it has likely been destructed", 1);
        fclose (logFileConst);
        free (stk->buffer);
        return STKDEAD;
    }

    for (size_t i = stk->nElement; i < stk->capacity; i++)
        if (stkBuffer (i) != stk->poison)
        {         
            LOGDUMP (logFileConst, stk, 0, "Poison values have failed - it means your stack has been accessed with non-stack function", 1);
            fclose (logFileConst);   
            free (stk->buffer);
            return STKOUTSIDEACCESS;
        }

    if (stk->nElement > stk->capacity)
    {
        LOGDUMP (logFileConst, stk, 0, "You have tried to access memory that is not bufferized (overflow)", 1);
        fclose (logFileConst);
        free (stk->buffer);
        return STKOVERFLOW;
    }

    LOGDUMP (logFileConst, stk, 1, "EVERYTHING IS OK", 0);

    fclose (logFileConst);

    return NOERR; 
}

/*hash_t countHash (struct Stk* stk)
{
    MY_ASSERT (stk != nullptr, "pointer to stk stuct is nullptr");

    static uint32_t seed = 0xBEB7AAAA;
    uint32_t hash = murmurhash (stk, (uint32_t) sizeof (stk), seed); // 0xb6d99cf8
    return hash;
}*/

void prepareLogs ()
{
    FILE* const temp = fopen ("log.txt", "w");
    
    MY_ASSERT (temp != nullptr, "Couldn't prepare logs (open the log.txt)");
    fclose (temp);
}

static hash_t rotl (hash_t n)
{
    unsigned d = 13;
    n *= d;
    return (n << d)|(n >> (32 - d));
}

template <typename data>
hash_t hashCalc (struct stk<data>* stk)
{
    MY_ASSERT (stk != nullptr, "pointer to stk struct equals nullptr");

    hash_t hash = 0xBEB7A000;

    hash ^= rotl (stk->canaryL);
    hash ^= rotl (stk->lastError);
    hash ^= rotl (stk->capacity);
    hash ^= rotl (stk->poison);

    if (stk->nElement > 0 && stk->buffer != nullptr)
        for (size_t i = 0; i < stk->nElement; ++i)
            hash ^= rotl (stkBuffer (i));

    hash ^= rotl (stk->canaryR);

    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}


template <typename data>
void dumpStk (struct stk<data>* stk, FILE* const logFileConst)
{
    fprintf (logFileConst, "Capacity = %zu\n", stk->capacity);
    fprintf (logFileConst, "nElement = %zu\n", stk->nElement);
    fprintf (logFileConst, "lastError = %d\n\n", stk->lastError);

    fprintf (logFileConst, "Lcanary = %llx\n", stk->canaryL);
    fprintf (logFileConst, "Lcanary in buffer = %llx\n\n", stkCanaryBufferL);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        //fprintf (logFileConst,  "buffer[%zu] = %d\n", i, stkBuffer (i));
        stk->dumper (stk, i, logFileConst); 

        if (i == stk->nElement - 1 && stk->nElement != stk->capacity)
            fprintf (logFileConst, "\n"
                     "Further elements do not contain stack elements,"
                     "but they lie in the bufferized region"
                     "\n"
                     "\n");
    }    

    fprintf (logFileConst, "\nRcanary = %llx\n", stk->canaryR);
    fprintf (logFileConst, "Rcanary in buffer = %llx\n", stkCanaryBufferR);
}

#endif
