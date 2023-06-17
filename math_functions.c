#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math_functions.h"

int solveEquation(int num1, char operator, int num2) 
{
    int result;

    switch (operator) 
    {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) //division by zero error
	    {
                fprintf(stderr, "Division by zero\n");
                exit(EXIT_FAILURE);
            }
            result = num1 / num2;
            break;
        default:
            fprintf(stderr, "Invalid operator\n");
            exit(EXIT_FAILURE);
    }

    return result;
}

