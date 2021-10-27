#include "stackFunctions.h"
#include "ultimateGuard.h"


int main ()
{
    struct stk<double> testStack = {};

    double poppedVal = -100;
    
    ctorStk (&testStack, 32.0, &dumpFunctionDouble); 

    //printStk (&testStack);

    pushStk<double> (&testStack, 1);
    //printStk (&testStack);

    pushStk<double> (&testStack, 28);
    //printStk (&testStack);

    pushStk<double> (&testStack, 28);
    //printStk (&testStack);
    //
    pushStk<double> (&testStack, 28);
    //printStk (&testStack); 

    pushStk<double> (&testStack, 228);
    //printStk (&testStack);
    
    pushStk<double> (&testStack, 1337);
    //printStk (&testStack);
   
    //printf ("ts nelement = %zu\n", testStack.nElement);

    popStk<double> (&testStack, &poppedVal);
    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    pushStk<double> (&testStack, 15);
    //printStk (&testStack);
    
    //printf ("\n%d\n", *testStack.buffer);

    //testStack.canaryL = 1;
    //testStack.canaryR = 2;

    //printf ("hash = %llx", countHash (stk));

    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);
    popStk<double> (&testStack, &poppedVal);

    //printStk (&testStack);
    //printf ("popped value = %d\n", poppedVal);

    dtorStk (&testStack); 
    return NOERR;
}
