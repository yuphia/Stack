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

const char splitter[] = "====================================================================================================================================\n";

//===========================================================================
//===========================================================================

#define stkBuffer(i) *(int*)((unsigned char*)stk->buffer + i*sizeof(int) + sizeof (canary_t)) 
#define stkCanaryBufferL *(canary_t*)(stk->buffer)
#define stkCanaryBufferR *(canary_t*)((unsigned char*)stk->buffer + stk->capacity*sizeof(int) + sizeof(canary_t)) 

//===========================================================================

#define STK_ZASHIBIS()\
    if (validityStk (stk, &info) != NOERR)\
    {\
        printf ("An error has occured, please turn on the debug mode and check log.txt\n");\
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
        fprintf (logFile, "dump #%zu\n\n", dumpCounter());\
        if (isError)\
        {\
            fprintf (logFile, "An error has occured in file: %s\n\n"\
                    "Detected in Line: %d \n\n"\
                    "While executing function: %s\n", info->file, info->line, info->function);\
            printf ("An error has occured, please turn on"\
                " the debug mode and check log.txt\n");\
            \
            fprintf (logFile, "\n"#message"\n\n");\
        }\
        else\
        {\
            fprintf (logFile, "Everything is OK\n\n");\
        }\
        \
        if (isPossibleToWrite)\
        {\
           dumpStk (stk, logFileConst);\
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
        fprintf (logFile, "dump #%zu\n\n", dumpCounter());\
        if (isError)\
        {\
            fprintf (stderr, "%s\n", splitter);\
            fprintf (stderr, "dump #%zu\n", dumpCounter());\
            fprintf (stderr, "An error has occured in file: %s\n\n"\
                    "In Line: %d \n\n"\
                    "While executing function: %s\n", info->file, info->line, info->function);\
            \
            fprintf (stderr, "\n"#message"\n\n");\
             \
            if (isPossibleToWrite)\
                printStk (stk);\
            \
            fprintf (stderr, "%s\n", splitter);\
            fflush (stderr);\
            fprintf (logFile, "An error has occured in file: %s\n\n"\
                    "Detected in Line: %d \n\n"\
                    "While executing function: %s\n", info->file, info->line, info->function);\
            \
            fprintf (logFile, "\n"#message"\n\n");\
        }\
        else\
        {\
            fprintf (logFile, "Everything is OK.\n\n");\
        }\
        \
        if (isPossibleToWrite)\
        {\
           dumpStk (stk, logFileConst);\
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
            abort();\
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

void dumpStk (struct stk* stk, FILE* const logFileConst);
size_t dumpCounter();
enum stkError validityStk (struct stk* stk, struct dumpInfo* info);
hash_t hashCalc (struct stk* stk);

#endif
