#include "ultimateGuard.h"
#include "stackFunctions.h"

enum stkError ctorStk (struct stk* stk, int poison)
{
    struct dumpInfo info = {};
    getinfo ();
        
    MY_ASSERT (stk != nullptr, "pointer to your stack equals to nullptr");
    
    stk->buffer = calloc (sizeof(int) + 2*sizeof(canary_t), 
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
    
    STK_ZASHIBIS();

    return NOERR;
}

enum stkError dtorStk (struct stk* stk)
{
    struct dumpInfo info = {};
    getinfo ();
    
    STK_ZASHIBIS(); 

    printStk (stk);

    stk->capacity = 0;
    stk->nElement = 0;

    free (stk->buffer); 
 
    stk->buffer = nullptr;   

    return NOERR;
}

enum stkError pushStk (struct stk *stk, /*stkType*/int value) 
{  
    struct dumpInfo info = {};
    getinfo ();
    
    STK_ZASHIBIS();

    if (stk->nElement >= stk->capacity)
        if (resizeStk (stk) != NOERR)
            return FATAL_ERROR;

    stkBuffer (stk->nElement) = value;

    stk->nElement++;

    STK_ZASHIBIS();    
    return NOERR;
}

enum stkError popStk (struct stk *stk, int* poppedVal)
{
    struct dumpInfo info = {};
    getinfo ();
    
    $
    STK_ZASHIBIS();
    $
    if (stk->nElement == 0)
        stk->lastError =  STKUNDERFLOW;

    STK_ZASHIBIS();

    stk->nElement--;
     
    *poppedVal = stkBuffer (stk->nElement);
    stkBuffer (stk->nElement) = stk->poison; 
    
    if (stk->nElement == stk->capacity/2)
        resizeStk (stk);
   
    STK_ZASHIBIS();

    return NOERR;
}

enum stkError resizeStk (struct stk *stk)
{
    struct dumpInfo info = {};
    getinfo ();
    
    STK_ZASHIBIS();

    if (stk->nElement == stk->capacity)
    {
        stk->buffer = realloc (stk->buffer, (stk->capacity)*2*sizeof(int) + 
                                                            2*sizeof(canary_t));
        if (stk->buffer == nullptr)
            stk->lastError =  REALLOCNOMEM;
   
        stk->capacity *= 2;
      
        stkCanaryBufferR = canaryBufferR; 
        
        for (size_t i = stk->nElement; i < stk->capacity; i++)
            stkBuffer (i) = stk->poison;

        STK_ZASHIBIS();

        return NOERR;
    }

    if (stk->nElement <= stk->capacity/2 && stk->capacity != 1)
    {
        stk->buffer = realloc (stk->buffer, 2*sizeof(canary_t) + 
                                                  (stk->capacity)*sizeof(int)/2);
        if (stk->buffer == nullptr)
            stk->lastError = REALLOCNOMEM;
            
        stk->capacity /= 2; 
    
        stkCanaryBufferR = canaryBufferR;

        STK_ZASHIBIS();

        return NOERR;
    }

    return RESIZEWRONGCALL;
}

enum stkError printStk (struct stk *stk)
{
    struct dumpInfo info = {};
    getinfo ();

    STK_ZASHIBIS();

    for (size_t i = 0; i < stk->nElement; i++)
    { 
        printf ("buffer[%zu] = %d\n", i, stkBuffer (i));
    }

    printf ("\n");
        
    fflush (stdin);

    STK_ZASHIBIS();

    return NOERR;
}

