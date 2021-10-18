#include "ultimateGuard.h"
#include "stackFunctions.h"

enum stkError ctorStk (struct stk* stk, int poison)
{
    prepareLogs();
    GET_INFO();
        
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
    stk->hash = hashCalc (stk);
    
    STK_ZASHIBIS();

    return NOERR;
}

enum stkError dtorStk (struct stk* stk)
{
    GET_INFO();
    
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

enum stkError popStk (struct stk *stk, int* poppedVal)
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
 
    if (stk->nElement == stk->capacity/2)
        resizeStk (stk, stk->capacity/2);
   
    
    STK_ZASHIBIS();

    return NOERR;
}

enum stkError resizeStk (struct stk *stk, size_t newSize)
{
    GET_INFO ();
    
    STK_ZASHIBIS ();    

    stk->buffer = realloc (stk->buffer, newSize*sizeof(int) + 2*sizeof(canary_t));
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

enum stkError printStk (struct stk *stk)
{
    GET_INFO();

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

