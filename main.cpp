#include "stackFunctions.h"


int main ()
{
    struct stk<double> testStack = {};

    double poppedVal = -100;
    
    ctorStk (&testStack, 32.0, &dumpFunctionDouble<double>); 

    //printStk (&testStack);

    pushStk (&testStack, (double)1);
    //printStk (&testStack);

    pushStk (&testStack, (double)28);
    //printStk (&testStack);

    pushStk (&testStack, (double)28);
    //printStk (&testStack);
    //
    pushStk (&testStack, (double)28);
    //printStk (&testStack); 

    pushStk (&testStack, (double)228);
    //printStk (&testStack);
    
    pushStk (&testStack, (double)1337);
    //printStk (&testStack);
   
    //printf ("ts nelement = %zu\n", testStack.nElement);

    popStk (&testStack, &poppedVal);
    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    pushStk (&testStack, (double)15);
    //printStk (&testStack);
    
    //printf ("\n%d\n", *testStack.buffer);

    //testStack.canaryL = 1;
    //testStack.canaryR = 2;

    //printf ("hash = %llx", countHash (stk));

    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    popStk (&testStack, &poppedVal);
    //popStk (&testStack, &poppedVal);
    //popStk (&testStack, &poppedVal);

    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    dtorStk (&testStack); 
    return NOERR;
}
