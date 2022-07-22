#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // open file
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // counter of number of images
    int counter = 0;
    FILE *jpeg = NULL;
    uint8_t buffer[512];
    char filename[8];

    // read to find jpegs. when EOF reached number 1 will change, breaking the while loop of 1.
    while ((fread(&buffer, 512, 1, card)) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            if ((buffer[3] & 0xf0) == 0xe0)
            {
                // if not first, close previous file
                if (!(counter == 0))
                {
                    fclose(jpeg);
                }

                // name of files, counter
                sprintf(filename, "%03i.jpg", counter);
                jpeg = fopen(filename, "w");
                counter = counter + 1;
            }
        }
        // write to file if jpeg found
        if (!(counter == 0))
        {
            fwrite(&buffer, 512, 1, jpeg);
        }
    }
    fclose(card);
    fclose(jpeg);
    return 0;

}