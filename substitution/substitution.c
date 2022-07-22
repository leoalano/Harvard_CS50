#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//get the key
int main(int argc, string argv[])
{
    //check the keylenght
    if (argc != 2)
    {
        printf("Error1: key must contain 26 CONTINUOUS unique letters\n");
        return 1;
    }
    
    //make key case insensitive
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        argv[1][i] = tolower(argv[1][i]);
    }

    int z = 0;
    int n;
    
    //check number of characters
    for (z = 0, n = strlen(argv[1]); z < n; z++)
    {
    }

    if (z < 26 || z > 26)
    {
        printf("Error2: key must contain 26 unique letters, not more, not less.\n");
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
        printf("Error3: you must input only alphabetical characters.\n");
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
                printf("Error4: you must not repeat characters.\n");
                return 1;
            }
        }


    }

    //get plaintext

    string plaintext = get_string("plaintext: ");

    //encipher and print

    int LETTERS[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    int BIGLETTERS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


    printf("ciphertext: ");

    for (int i = 0, f = strlen(plaintext); i < f; i++)
    {
        char cipher;
        int count = 0;

        for (int y = 0; y < 26; y++)
        {

            if (plaintext[i] == LETTERS[y])//create cipher
            {
                cipher = argv[1][y];
                printf("%c", cipher);
            }

            else if (plaintext[i] == BIGLETTERS[y])//create cipher for 
            {
                cipher = argv[1][y];
                printf("%c", toupper(cipher));
            }

            else if (plaintext[i] != LETTERS[y] && plaintext[i] != BIGLETTERS[y])
            {
                count = count + 1;
            }
        }

        if (count == 26) 
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}