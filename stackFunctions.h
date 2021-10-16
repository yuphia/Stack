#include "myAssert.h"

#define $ printf ("line = %d in file: " __FILE__ "\n\n" , __LINE__);

enum stkError  {
                    NOERR = 0,
                    ALLOCNOMEM,
                    REALLOCNOMEM,
                    STKUNDERFLOW,
                    STKOVERFLOW,
                    STKDEAD,
                    RESIZEWRONGCALL,
                    WRONGPOINTERSTRUCT,
                    WRONGPOINTERBUFFER   
                };

struct stk 
        {
        int* buffer; //buffer[capacity]

        size_t nElement;
        size_t capacity;

        int poison;
//        size_t elementSize;
        };

enum stkError resizeStk (struct stk *stk);
enum stkError ctorStk (struct stk* stk, size_t requestedCapacity, int poison);
enum stkError dtorStk (struct stk* stk);
enum stkError pushStk (struct stk *stk, /*stkType*/int value); 
enum stkError popStk (struct stk *stk, int* poppedVal);
enum stkError printStk (struct stk *stk);
enum stkError validityStk (struct stk* stk);

enum stkError ctorStk (struct stk* stk, size_t requestedCapacity, int poison)
{
    MY_ASSERT (stk != nullptr, "pointer to your stack equals to nullptr");

    stk->buffer = (int*)calloc (requestedCapacity, sizeof(int));

    //printf ("value in stk = %d\n", *stk->buffer);

    if (stk->buffer == nullptr)
        return ALLOCNOMEM;

    stk->nElement = 0;
    stk->capacity = requestedCapacity;

    stk->poison = poison;

    return NOERR; // error code NOERR
}

enum stkError dtorStk (struct stk* stk)
{
    // CHECK STACK VALIDITY AND THAT IT HAS NOT BEEN DESTRUCTED
    MY_ASSERT (stk != nullptr, "pointer to stack equals to nullptr");

    for (size_t stkElement = 0; stkElement < (stk->capacity); stkElement++)
    {
        *(stk->buffer + stkElement) = stk->poison;
    }

    stk->capacity = 0;
    stk->nElement = 0;

    free (stk->buffer); 
        
    return NOERR; // error codeЖ
}

enum stkError pushStk (struct stk *stk, /*stkType*/int value) 
{
    MY_ASSERT (stk != nullptr, "pointer to stack equals nullptr");

    if (stk->nElement >= stk->capacity)
        resizeStk (stk);

    *(stk->buffer + stk->nElement) = value;
    stk->nElement++;
    
    return NOERR;
}

enum stkError popStk (struct stk *stk, int* poppedVal)
{
    MY_ASSERT (stk != nullptr, "pointer to stack equals null");

    if (stk->nElement == 0)
        return STKUNDERFLOW;
     
    *poppedVal = *(stk->buffer + stk->nElement - 1);
    *(stk->buffer + stk->nElement - 1) = 0;
    stk->nElement--;

    if (stk->nElement == stk->capacity/2)
        resizeStk (stk);

    return NOERR;
}

enum stkError resizeStk (struct stk *stk)
{
    MY_ASSERT (stk != nullptr, "pointer to stack equals null");

    if (stk->nElement == stk->capacity)
    {
        stk->buffer = (int*)realloc (stk->buffer, (stk->capacity)*2*sizeof(int));
        if (stk->buffer == nullptr)
            return REALLOCNOMEM;
    
        for (size_t i = stk->capacity + 1; i < stk->capacity*2; i++)
            *(stk->buffer + i) = 0;

        stk->capacity *= 2;

        return NOERR;
    }

    if (stk->nElement == stk->capacity/2 && stk->capacity != 1)
    {
        stk->buffer = (int*)realloc (stk->buffer, (stk->capacity)*sizeof(int)/2);
        if (stk->buffer == nullptr)
            return REALLOCNOMEM;

        stk->capacity /= 2;

        return NOERR;
    }

    return RESIZEWRONGCALL;
}

enum stkError printStk (struct stk *stk)
{
    MY_ASSERT (stk != nullptr, "pointer to stack equals null");

    for (size_t i = 0; i < stk->nElement; i++)
    {
        printf ("buffer[%zu] = %d\n", i, *(stk->buffer + i));
    }

    printf ("\n");

    return NOERR;
}

enum stkError validityStk (struct stk* stk)
{
    if (stk == nullptr)
        return WRONGPOINTERSTRUCT;

    if (stk->buffer == nullptr)
        return WRONGPOINTERBUFFER;

    if (stk->capacity == 0)
        return STKDEAD;

    if (stk->nElement > stk->capacity)
        return STKOVERFLOW;

    return NOERR; 
}
