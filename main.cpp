#include "stackFunctions.h"
#include "ultimateGuard.h"

int main ()
{
    struct stk testStack = {};

    int poppedVal = -100;
    
    ctorStk (&testStack, 32); 

    //printStk (&testStack);

    pushStk (&testStack, 1);
    //printStk (&testStack);

    pushStk (&testStack, 28);
    //printStk (&testStack);

    pushStk (&testStack, 28);
    //printStk (&testStack);
    //
    pushStk (&testStack, 28);
    //printStk (&testStack); 

    pushStk (&testStack, 228);
    //printStk (&testStack);
    
    pushStk (&testStack, 1337);
    //printStk (&testStack);
   
    //printf ("ts nelement = %zu\n", testStack.nElement);

    popStk (&testStack, &poppedVal);
    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    pushStk (&testStack, 15);
    //printStk (&testStack);
    
    //printf ("\n%d\n", *testStack.buffer);

    //testStack.canaryL = 1;
    //testStack.canaryR = 2;

    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);

    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    dtorStk (&testStack); 
    return NOERR;
}
