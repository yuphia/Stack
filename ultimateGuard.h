/** \file
    \brief Header file that defines macro MY_ASSERT, if flags DEBUG_SOFT and NDEBUG are not defined
    MY_ASSERT (statement, message) checks the statement and if it is false prints the message
    Also it prints the line, function and file where the error has occured
    After printing it aborts the programm

    If DEBUG_SOFT flag is defined does the same but doesn't abort the programm

    If NDEBUG flag is defined does nothing**/

#pragma once

#include <stdio.h>
#include <math.h>
#include "stackFunctions.h"
//#if defined(deff1) || defined(def2)

FILE* const logFileConst = fopen ("log.txt", "w");

const char splitter[] = "====================================================================================================================================\n";

//===========================================================================
//===========================================================================

#define stackBuffer(i) *(stk->buffer + i) 
#define stackCanaryBufferL *(stk->buffer)
#define stackCanaryBufferR *(stk->buffer + stk->capacity - sizeof(canary_t)) 

//===========================================================================

#define STK_ZASHIBIS()\
    if (validityStk (stk) != NOERR)\
    {\
        printf ("An error has occured, please turn on"\
                " the debug mode and check log.txt\n");\
        return FATAL_ERROR;\
    }            

#define $ printf ("line = %d in file: " __FILE__ "\n\n" , __LINE__);

//===========================================================================

#ifdef LOGDUMP
    #undef LOGDUMP
#endif

#ifndef NDEBUG_STK
    #define LOGDUMP(logFile, stk, isPossibleToWrite, message, isError) \
    {\
        fprintf (logFile, "%s\n", splitter);\
        fprintf (logFile, "dump #%zu\n", dumpCounter());\
        if (isError)\
        {\
            \
            fprintf (logFile, "An error has occured in file:" __FILE__ "\n\n"\
                    "In Line: %d \n\n"\
                    "While executing function: %s\n" , __LINE__, __PRETTY_FUNCTION__);\
            \
            fprintf (logFile, "\n"#message"\n\n");\
        }\
        \
        if (isPossibleToWrite)\
        {\
           dumpStk (stk);\
           fflush (logFile);\
        }\
        else\
        {\
           fprintf (logFile, "It is imposibble to dump the stack elements due to the error");\
        }\
        \
        fprintf (logFile, "%s", splitter);\
    }
#endif

#ifdef NDEBUG_STK
    #define LOGDUMP(logFile, stk, isPossibleToWrite, message)
#endif

//===========================================================================

void dumpStk (struct stk* stk);
size_t dumpCounter();
enum stkError validityStk (struct stk* stk);


void dumpStk (struct stk* stk)
{
    for (size_t i = 0; i < stk->capacity; i++)
    {
        fprintf (logFileConst,  "buffer[%zu] = %d\n", i, *(stk->buffer + i));

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

    if (stk->buffer == nullptr && stk->capacity != 0)
    {
        LOGDUMP (logFileConst, stk, 0, "Pointer to buffer equals nullptr. It is possible allocation or reallocation has failed\n", 1);
        return ALLOCNOMEM;
    }

    if (stk->capacity == 0 && stk->buffer == nullptr)
    {
        LOGDUMP (logFileConst, stk, 0, "The capacity of the stack equal to null - which means it has likely been destructed", 1);
        return STKDEAD;
    }

    for (size_t i = stk->nElement; i < stk->capacity; i++)
        if (*(stk->buffer + i) != stk->poison)
        {
            LOGDUMP (logFileConst, stk, 1, "Poison values have failed - it means your stack has been accessed with non-stack function", 1);
            return STKOUTSIDEACCESS;
        }

    if (stk->nElement > stk->capacity)
    {
        LOGDUMP (logFileConst, stk, 0, "You have tried to access memory that is not bufferized (overflow)", 1);
        return STKOVERFLOW;
    }

    LOGDUMP (logFileConst, stk, 1, "EVERYTHING IS OK", 0);

    return NOERR; 
}
