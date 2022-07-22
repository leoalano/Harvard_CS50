#include <stdio.h>
#include <cs50.h>

bool valid_triangle (int a, int b, int c);

int main (void)
{ 
    //get input
    int x = get_int ("side x\n");
    int y = get_int ("side y\n");
    int z = get_int ("side z\n");
    
    
    bool valid_triangle (int a, int b, int c);
   
    
}

bool valid_triangle (int a, int b, int c)
{ 
     if (a + b <= c || a + c <= b || b + c <= a)
    {
        return false;
        printf("false");
    } 
    
    else
    {
        return true;
        printf("true");
    }
}

        
    
