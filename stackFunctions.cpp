#include "ultimateGuard.h"
#include "stackFunctions.h"

enum stkError ctorStk (struct stk* stk, size_t requestedCapacity, int poison)
{
    MY_ASSERT (stk != nullptr, "pointer to your stack equals to nullptr");

    stk->buffer = (int*)calloc (requestedCapacity, sizeof(int));

    //printf ("value in stk = %d\n", *stk->buffer);
    //
    //printf ("pointer to stack = %p\n", (void*)stk->buffer);

    if (stk->buffer == nullptr)
        return ALLOCNOMEM;
    
    stk->canaryL = canaryL;
    
    *(stk->buffer) = poison;
    
    stk->nElement = 0;
    stk->capacity = requestedCapacity;
    
    stk->lastError = NOERR;

    stk->canaryR = canaryR;

    stk->poison = poison;
    
    return NOERR;
}

enum stkError dtorStk (struct stk* stk)
{
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
    STK_ZASHIBIS();

    if (stk->nElement >= stk->capacity)
        if (resizeStk (stk) != NOERR)
            return FATAL_ERROR;

    *(stk->buffer + stk->nElement) = value;

    stk->nElement++;

    STK_ZASHIBIS();    
    return NOERR;
}

enum stkError popStk (struct stk *stk, int* poppedVal)
{
    STK_ZASHIBIS();
    
    if (stk->nElement == 0)
        return STKUNDERFLOW;
     
    *poppedVal = *(stk->buffer + stk->nElement - 1);
    *(stk->buffer + stk->nElement - 1) = stk->poison;
    stk->nElement--;

    if (stk->nElement == stk->capacity/2)
        resizeStk (stk);
   
    STK_ZASHIBIS();

    return NOERR;
}

enum stkError resizeStk (struct stk *stk)
{
    STK_ZASHIBIS();

    if (stk->nElement == stk->capacity)
    {
        stk->buffer = nullptr;//(int*)realloc (stk->buffer, (stk->capacity)*2*sizeof(int));
        if (stk->buffer == nullptr)
            stk->lastError =  REALLOCNOMEM;
    
        STK_ZASHIBIS();
       
        for (size_t i = stk->capacity; i < stk->capacity*2; i++)
            *(stk->buffer + i) = stk->poison;

        stk->capacity *= 2;

        STK_ZASHIBIS();

        return NOERR;
    }

    if (stk->nElement == stk->capacity/2 && stk->capacity != 1)
    {
        stk->buffer = (int*)realloc (stk->buffer, (stk->capacity)*sizeof(int)/2);
        if (stk->buffer == nullptr)
            stk->lastError = REALLOCNOMEM;
            
        stk->capacity /= 2;

        STK_ZASHIBIS();

        return NOERR;
    }

    return RESIZEWRONGCALL;
}

enum stkError printStk (struct stk *stk)
{
    STK_ZASHIBIS();

    for (size_t i = 0; i < stk->nElement; i++)
    {
        printf ("buffer[%zu] = %d\n", i, *(stk->buffer + i));
    }

    printf ("\n");
        
    fflush (stdin);

    STK_ZASHIBIS();

    return NOERR;
}


