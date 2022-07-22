#include<cs50.h>
#include<stdio.h>

int main(void)
// get input from user
{
    int input;

    do
    {
        input = get_int("height\n");
    }

    while (input < 1 || input > 8);
    
//establish variables
    int x = 0;
    
    int input1 = input + 1;
    
//major loop
    for (x = 1; x < input1; x++)
    { 
        int spaces = input - x;
//inner loops
        for (int i = 0; i < spaces; i++)
        {
            printf(" ");
        }
    
        for (int j = 0; j < x; j++)
        {
            printf("#");
        }
  
        printf("  ");
    
        for (int j = 0; j < x; j++)
        {
            printf("#");
        }
    
        printf("\n"); 
    }
}
    