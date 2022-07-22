#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string phrase);

int main(void)
{
    //input from user
    string input = get_string("Text: ");

    //calculate number of letters (no symbols)
    int letters = count_letters(input);

    //count number of words
    int words = 0;

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == 32)
        {
            words = words + 1;
        }
    }
    words = words + 1;


    //count number of sentences

    int sentences = 0;

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == '.' || input[i] == '?' || input[i] == '!')
        {
            sentences = sentences + 1;
        }
    }


    //calculate grade of text
    float averagelet = ((float) letters / (float) words) * 100;

    float averagewor = ((float) sentences / (float) words) * 100;

    float index = (0.0588 * averagelet) - (0.296 * averagewor) - 15.8;

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else
    {
        printf("Grade %i\n", (int) round(index));
    }


}

int count_letters(string phrase)
{
    //total number of characters
    int chars = strlen(phrase);

    //count number of nonletters
    int nonletters = 0;

    for (int i = 0, f = strlen(phrase); i < f; i++)
    {
        if (phrase[i] < 97 && phrase[i] > 90)
        {
            nonletters = nonletters + 1;
        }

        if (phrase[i] < 65 || phrase[i] > 122)
        {
            nonletters = nonletters + 1;
        }

    }
    //calculate total letters only
    int letters = chars - nonletters;

    return letters;

}