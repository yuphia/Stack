#ifndef STACKFUNCTIONS_H
#define STACKFUNCTIONS_H 1

#include "myAssert.h"
#include <math.h>
#include <stdio.h>

#define stkBuffer(i) *(data*)((unsigned char*)stk->buffer + i*sizeof(data) + sizeof (canary_t))
#define stkCanaryBufferL *(canary_t*)(stk->buffer)
#define stkCanaryBufferR *(canary_t*)((unsigned char*)stk->buffer + stk->capacity*sizeof(data) + sizeof(canary_t))


#define GET_INFO() struct dumpInfo info = {};\
    info.line = __LINE__;\
    info.function = __PRETTY_FUNCTION__;\
    info.file = __FILE__;\

#define STK_ZASHIBIS()\
    if (validityStk (stk, &info) != NOERR)\
    {\
        printf ("An error has occured, please turn on the debug mode and check log.txt\n");\
        return FATAL_ERROR;\
    }
//#if defined(deff1) || defined(def2)

const char splitter[] = "====================================================================================================================================\n";

//===========================================================================
//===========================================================================

//===========================================================================
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



typedef unsigned long long canary_t;
typedef unsigned long long hash_t;

const canary_t canaryL        = 0xABADBABE;
const canary_t canaryR        = 0xB16B00B5;
const canary_t canaryBufferL  = 0xDEFEC8ED;
const canary_t canaryBufferR  = 0xDEAD2BAD;

const size_t stkDelta = 2;

enum stkError
                {
                    NOERR = 0,
                    ALLOCNOMEM = -100,
                    REALLOCNOMEM = -101,
                    STKUNDERFLOW = -1,
                    STKOVERFLOW = 228,
                    STKOUTSIDEACCESS = 1337,
                    STKDEAD = 4,
                    RESIZEWRONGCALL = 1989,
                    NULLPOINTERSTRUCT = 7,
                    FATAL_ERROR = 666,
                    NON_FATAL_ERROR = 616,
                    CANARYL_DEAD = 10,
                    CANARYR_DEAD = 15,
                    CANARYL_BUFF_STK_DEAD = 20,
                    CANARYR_BUFF_STK_DEAD = 25,
                    HASH_DEAD = 100
                };

struct dumpInfo
{
    int line;
    const char* file;
    const char* function;
};

template <typename data>
    struct stk
        {
            canary_t canaryL;

            void* buffer; //buffer[capacity]

            size_t nElement;
            size_t capacity;

            data poison;

            enum stkError lastError;

            hash_t hash;

            void (*dumper) (struct stk* stk, size_t i, FILE* const logFileConst);

            canary_t canaryR;
        };


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
enum stkError resizeStk (struct stk<data> *stk, size_t newSize);

template <typename data>
enum stkError ctorStk (struct stk<data>* stk, data poison, void (*dumper) (struct stk<data>* stk, size_t i, FILE* const logFileConst));

template <typename data>
enum stkError dtorStk (struct stk<data>* stk);

template <typename data>
enum stkError pushStk (struct stk<data> *stk, /*stkType*/data value);

template <typename data>
enum stkError popStk (struct stk<data> *stk, data* poppedVal);

template <typename data>
enum stkError printStk (struct stk<data> *stk);


void prepareLogs();

template <typename data>
enum stkError ctorStk (struct stk<data>* stk, data poison, void (*dumper) (struct stk<data>* stk, size_t i, FILE* const logFileConst))
{
    prepareLogs();
    GET_INFO();

    MY_ASSERT (stk != nullptr, "pointer to your stack equals to nullptr");

    stk->buffer = calloc (sizeof(data) + 2*sizeof(canary_t),
                                          sizeof(unsigned char));

    //printf ("value in stk = %d\n", *stk->buffer);
    //
    //printf ("pointer to stack = %p\n", (void*)stk->buffer);

    if (stk->buffer == nullptr)
        return ALLOCNOMEM;

    stk->canaryL = canaryL;
    stk->capacity = 1;

    stkCanaryBufferL = canaryBufferL;
    stkCanaryBufferR = canaryBufferR;

    for (size_t i = 0; i < stk->capacity; i++)
        stkBuffer (i) = poison;

    stk->nElement = 0;
    stk->lastError = NOERR;
    stk->canaryR = canaryR;
    stk->poison = poison;
    stk->hash = hashCalc (stk);
    stk->dumper = dumper;

    STK_ZASHIBIS();

    return NOERR;
}

template <typename data>
enum stkError dtorStk (struct stk<data>* stk)
{
    GET_INFO();

    STK_ZASHIBIS();

    stk->capacity = 0;
    stk->nElement = 0;

    free (stk->buffer);

    stk->buffer = nullptr;

    return NOERR;
}

template <typename data>
enum stkError pushStk (struct stk<data> *stk, /*stkType*/data value)
{
    GET_INFO ();

    STK_ZASHIBIS();

    if (stk->nElement >= stk->capacity)
        if (resizeStk (stk, 2*stk->capacity) != NOERR)
            return FATAL_ERROR;

    stkBuffer (stk->nElement) = value;

    stk->nElement++;

    stk->hash = hashCalc (stk);

    STK_ZASHIBIS();
    return NOERR;
}

template <typename data>
enum stkError popStk (struct stk<data> *stk, data* poppedVal)
{
    GET_INFO();

    STK_ZASHIBIS();

    if (stk->nElement == 0)
        stk->lastError =  STKUNDERFLOW;

    STK_ZASHIBIS();

    stk->nElement--;

    *poppedVal = stkBuffer (stk->nElement);
    stkBuffer (stk->nElement) = stk->poison;

    stk->hash = hashCalc (stk);

    if (stk->nElement == stk->capacity/2 - stkDelta)
        resizeStk (stk, stk->capacity/2);


    STK_ZASHIBIS();

    return NOERR;
}

template <typename data>
enum stkError resizeStk (struct stk<data> *stk, size_t newSize)
{
    GET_INFO ();

    STK_ZASHIBIS ();

    stk->buffer = realloc (stk->buffer, newSize*sizeof(data) + 2*sizeof(canary_t));
    if (stk->buffer == nullptr)
        stk->lastError =  REALLOCNOMEM;

    stk->capacity = newSize;

    stkCanaryBufferR = canaryBufferR;

    for (size_t i = stk->nElement; i < stk->capacity; i++)
        stkBuffer (i) = stk->poison;

    stk->hash = hashCalc (stk);

    STK_ZASHIBIS();

    return NOERR;
}

template <typename data>
enum stkError printStk (struct stk<data> *stk)
{
    GET_INFO();

    STK_ZASHIBIS();

    for (size_t i = 0; i < stk->nElement; i++)
    {
        //printf ("buffer[%zu] = %d\n", i, stkBuffer (i));
    }

    printf ("\n");

    fflush (stdin);

    STK_ZASHIBIS();

    return NOERR;
}

size_t dumpCounter()
{
    static size_t dumpCounter = 0;

    dumpCounter++;
    return dumpCounter;
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
void dumpFunctionInt (struct stk<data>* stk, size_t i, FILE* const logFileConst)
{
    fprintf (logFileConst,  "buffer[%zu] = %d\n", i, stkBuffer (i));
}

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

void prepareLogs ()
{
    fclose (fopen ("log.txt", "w"));
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

template <typename data>
void dumpFunctionDouble (struct stk<data>* stk, size_t i, FILE* const logFileConst)
{
    fprintf (logFileConst,  "buffer[%zu] = %lg\n", i, stkBuffer (i));
}
#endif




