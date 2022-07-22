#include<stdio.h>
#include<cs50.h>

    //ask for name 
int main(void)
{
    string name = get_string ("what's your name?\n");
    printf("hello, %s\n", name);
}