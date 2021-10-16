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

#if (defined(NDEBUG_STK))&&(!defined(DEBUG_STK))
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
        }\
        else\
        {\
           fprintf (logFile, "It is imposibble to dump the stack elements due to the error\n");\
        }\
        \
        fprintf (logFile, "%s\n", splitter);\
        fflush (logFile);\
    }
#endif

#if (!defined(NDEBUG_STK))&&(defined(DEBUG_STK))
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
        }\
        else\
        {\
           fprintf (logFile, "It is imposibble to dump the stack elements due to the error\n");\
        }\
        \
        fprintf (logFile, "%s\n", splitter);\
        fflush (logFile);\
        \
        if (isError)\
        {\
            fprintf (stderr, "%s\n", splitter);\
            fprintf (stderr, "dump #%zu\n", dumpCounter());\
            fprintf (stderr, "An error has occured in file:" __FILE__ "\n\n"\
                    "In Line: %d \n\n"\
                    "While executing function: %s\n" , __LINE__, __PRETTY_FUNCTION__);\
            \
            fprintf (stderr, "\n"#message"\n\n");\
             \
            if (isPossibleToWrite)\
            {\
                dumpStk (stk);\
            }\
            else\
            {\
                fprintf (logFile, "It is imposibble to dump the stack elements due to the error\n");\
            }\
            \
            fprintf (stderr, "%s\n", splitter);\
            fflush (stderr);\
            abort();\
        }\
    }
#endif

#if (defined(NDEBUG_STK) && defined(DEBUG_STK)) || (!defined(NDEBUG_STK) && !defined(DEBUG_STK)) 
    #define LOGDUMP(logFile, stk, isPossibleToWrite, message, isError)\
    {\
        printf("Running options seem to be wrong, you should either run with"\
                " DEBUG_STK or NDEBUG_STK, please use the Makefile to compile and run"\
                ", If there is still a mistake please check Makefile options\n");\
        abort();\
    }
#endif

//===========================================================================

void dumpStk (struct stk* stk);
size_t dumpCounter();
enum stkError validityStk (struct stk* stk);



#endif
