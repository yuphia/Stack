#include "myAssert.h"
#include "ultimateGuard.h"

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





