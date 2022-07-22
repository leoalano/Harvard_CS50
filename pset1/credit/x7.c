#include<cs50.h>
#include<stdio.h>

int main(void)
{
//user input
    long input;

    input = get_long("Number:");


// calculate checksum

    long x1 = input%10; //x1 is the last number (counting from right to left)
    long x2 = (input/10)%10;
    long x3 = (input/100)%10;
    long x4 = (input/1000)%10;
    long x5 = (input/10000)%10;
    long x6 = (input/100000)%10;
    long x7 = (input/1000000)%10;
    long x8 = (input/10000000)%10;
    long x9 = (input/100000000)%10;
    long x10 = (input/1000000000)%10;
    long x11 = (input/10000000000)%10;
    long x12 = (input/100000000000)%10;
    long x13 = (input/1000000000000)%10;
    long x14 = (input/10000000000000)%10;
    long x15 = (input/100000000000000)%10;
    long x16 = (input/1000000000000000)%10;


    int sum = (x2*2)+(x4*2)+(x6*2)+(x8*2)+(x10*2)+(x12*2)+(x14*2)+(x16*2);
    int luhn = sum + x1 + x3 + x5 + x7 + x9 + x11 + x13 + x15;

    printf("%li\n", x16);


} 