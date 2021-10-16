/** \file
    \brief Header file that defines macro MY_ASSERT, if flags DEBUG_SOFT and NDEBUG are not defined
    MY_ASSERT (statement, message) checks the statement and if it is false prints the message
    Also it prints the line, function and file where the error has occured
    After printing it aborts the programm

    If DEBUG_SOFT flag is defined does the same but doesn't abort the programm

    If NDEBUG flag is defined does nothing**/

#pragma once

#include <stdio.h>
#include <math.h>

//#if defined(deff1) || defined(def2)

#ifdef MY_ASSERT
    #undef MY_ASSERT
#endif

#ifndef DEBUG_SOFT
    #ifndef NDEBUG
        #define MY_ASSERT(statement, message) \
        if (!(statement))\
        {\
            printf ("\v" #message "\n\n");\
            printf ("An error has occured in file:" __FILE__ "\n\n"\
                    "In Line: %d \n\n"\
                    "While executing function: %s\n\n" , __LINE__, __PRETTY_FUNCTION__);\
            abort();\
        }
    #endif
#endif

#ifdef DEBUG_SOFT
    #define MY_ASSERT(statement, message) \
        if (!(statement))\
        {\
            printf ("\v" #message "\n\n");\
            printf ("An error has occured in file:" __FILE__ "\n\n"\
                    "In Line: %d \n\n"\
                    "While executing function: %s\n\n" , __LINE__, __PRETTY_FUNCTION__);\
        }
#endif

#ifdef NDEBUG
    #define MY_ASSERT(statement, message)
#endif
