#include<cs50.h>
#include<stdio.h>

int main(void)
{
//user input
    long input;

    input = get_long("Number:");


// calculate checksum

    long x1 = input % 10; //x1 is the last number (counting from right to left)
    long x2 = 2 * ((input / 10) % 10);
    long x3 = (input / 100) % 10;
    long x4 = 2 * ((input / 1000) % 10);
    long x5 = (input / 10000) % 10;
    long x6 = 2 * ((input / 100000) % 10);
    long x7 = (input / 1000000) % 10;
    long x8 = 2 * ((input / 10000000) % 10);
    long x9 = (input / 100000000) % 10;
    long x10 = 2 * ((input / 1000000000) % 10);
    long x11 = (input / 10000000000) % 10;
    long x12 = 2 * ((input / 100000000000) % 10);
    long x13 = (input / 1000000000000) % 10;
    long x14 = 2 * ((input / 10000000000000) % 10);
    long x15 = (input / 100000000000000) % 10;
    long x16 = 2 * ((input / 1000000000000000) % 10);

//below (x2a, x2b, etc) refers to separating the digits of the numbers in x2, x4, etc. 
    long x2a = (x2 % 10);
    long x2b = (x2 / 10) % 10;

    long x4a = (x4 % 10);
    long x4b = (x4 / 10) % 10;

    long x6a = (x6 % 10);
    long x6b = (x6 / 10) % 10;

    long x8a = (x8 % 10);
    long x8b = (x8 / 10) % 10;

    long x10a = (x10 % 10);
    long x10b = (x10 / 10) % 10;

    long x12a = (x12 % 10);
    long x12b = (x12 / 10) % 10;

    long x14a = (x14 % 10);
    long x14b = (x14 / 10) % 10;

    long x16a = (x16 % 10);
    long x16b = (x16 / 10) % 10;

    int sum = x2a + x2b + x4a + x4b + x6a + x6b + x8a + x8b + x10a + x10b + x12a + x12b + x14a + x14b + x16a + x16b; 
    
    int luhn = sum + x1 + x3 + x5 + x7 + x9 + x11 + x13 + x15; //luhn is the verification number. If ending in 0 good to go

    
//check for card lenght and starting digits

//Print Amex, visa, mastercard or invalid



    if (luhn % 10 != 0)
    {
        printf("INVALID\n");
    }

    else if (input >= 340000000000000 & input < 350000000000000 || input >= 370000000000000 & input < 380000000000000)
    {
        printf("AMEX\n");
    }

    else if (input >= 4000000000000000 & input < 5000000000000000 || input >= 4000000000000 & input < 5000000000000)
    {
        printf("VISA\n");
    }

    else if (input >= 5100000000000000 & input < 5600000000000000)
    {
        printf("MASTERCARD\n");
    }

    else
    {
        printf("INVALID\n");
    }


}
