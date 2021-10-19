#ifndef STACKFUNCTIONS_H
    #define STACKFUNCTIONS_H 1 

    #include "myAssert.h"

    #define GET_INFO()\
    \
        struct dumpInfo info = {};\
        info.line = __LINE__;\
        info.function = __PRETTY_FUNCTION__;\
        info.file = __FILE__;\
    \

    typedef unsigned long long canary_t;
    typedef unsigned long long hash_t;

    const canary_t canaryL        = 0xABADBABE;
    const canary_t canaryR        = 0xB16B00B5;
    const canary_t canaryBufferL  = 0xDEFEC8ED;
    const canary_t canaryBufferR  = 0xDEAD2BAD;

    const size_t stkDelta = 2;

    enum stkError  {
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

    struct stk 
        {
            canary_t canaryL;

            void* buffer; //buffer[capacity]
    
            size_t nElement;
            size_t capacity;

            int poison;

            enum stkError lastError;

            hash_t hash;

            canary_t canaryR;
        };

    enum stkError resizeStk (struct stk *stk, size_t newSize);
    enum stkError ctorStk (struct stk* stk, int poison);
    enum stkError dtorStk (struct stk* stk);
    enum stkError pushStk (struct stk *stk, /*stkType*/int value); 
    enum stkError popStk (struct stk *stk, int* poppedVal);
    enum stkError printStk (struct stk *stk);
    void prepareLogs();
#endif




