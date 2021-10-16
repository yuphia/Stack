#ifndef STACKFUNCTIONS_H
    #define STACKFUNCTIONS_H 1

    #include "myAssert.h"


    typedef unsigned long long canary_t;

    const canary_t canaryL        = 0xABADBABE;
    const canary_t canaryR        = 0xB16B00B5;
    const canary_t canaryBufferL  = 0xDEFEC8ED;
    const canary_t canaryBufferR  = 0xDEAD2BAD;

    enum stkError  {
                    NOERR = 0,
                    ALLOCNOMEM = -100,
                    STKUNDERFLOW = -1,
                    STKOVERFLOW = 228,
                    STKOUTSIDEACCESS = 1337,
                    STKDEAD = 4,
                    RESIZEWRONGCALL = 1989,
                    NULLPOINTERSTRUCT = 7, 
                    FATAL_ERROR = 666  
                    };
    
    struct stk 
            {
            canary_t canaryL;

            int* buffer; //buffer[capacity]
    
            size_t nElement;
            size_t capacity;

            int poison;

            canary_t canaryR;
            };

    enum stkError resizeStk (struct stk *stk);
    enum stkError ctorStk (struct stk* stk, size_t requestedCapacity, int poison);
    enum stkError dtorStk (struct stk* stk);
    enum stkError pushStk (struct stk *stk, /*stkType*/int value); 
    enum stkError popStk (struct stk *stk, int* poppedVal);
    enum stkError printStk (struct stk *stk);
#endif




