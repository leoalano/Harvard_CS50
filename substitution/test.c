#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

    //get the key
int main(int argc, string argv[])
{

        //check the keylenght
    if (argc > 2)
    {
        printf("Error: key must contain 26 CONTINUOUS unique letters\n");
        return 1;
    }

    int z = 0;
    int n;

    for (z = 0, n = strlen(argv[1]); z < n; z++)
    {
    }

    if (z < 26 || z > 26)
        {
            printf("Error: key must contain 26 unique letters, not more, not less.\n");
            return 1;
        }


        //check for non alphabetic characters
    int x = 0;

    for (int i = 0; i < n; i++)
    {
        if ((argv[1][i] < 97 && argv[1][i] > 90) || (argv[1][i] < 65 || argv[1][i] > 122))
        {
            x = x + 1;
        }
    }
    if (x != 0)
    {
        printf("Error: you must input only alphabetical characters.\n");
        return 1;
    }


        //check for repeated characters


    for (int i = 0; i < n; i++)
    {
        for (int r = i + 1; r < n; r++)
        {
            int repeat = argv[1][i] - argv[1][r];

            if (repeat == 0)
            {
                printf("Error: you must not repeat characters.\n");
                return 1;
            }
        }


    }

    //get plaintext

    string input = get_string("plaintext: ");

    //encipher and print

    int LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    printf("ciphertext: "); 
    
    if (input[1] == 98)
    {
        printf("deu certo");  
    }
}