#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get numbers by user
    int x = get_int("x: ");
    int y = get_int("y: ");
    
    //divide x / y
    float z = (float) x / (float) y;
    printf("%f\n", z);
    
    if (z > 10) 
        {printf("Congratulations");
        }
    else
        {printf("you lost!");
        }
}