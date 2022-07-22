#include "helpers.h"
#include <stdio.h>
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average[height][width];
    //find average
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            average[i][x] = ((float)image[i][x].rgbtRed + (float)image[i][x].rgbtGreen + (float)image[i][x].rgbtBlue) / 3;

            // rounding values
            image[i][x].rgbtRed = lround(average[i][x]);
            image[i][x].rgbtGreen = lround(average[i][x]);
            image[i][x].rgbtBlue = lround(average[i][x]);
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaRed[height][width];
    float sepiaGreen[height][width];
    float sepiaBlue[height][width];

    //calculate sepia
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            sepiaRed[i][x] = ((((double)image[i][x].rgbtRed) * .393) + (((double)image[i][x].rgbtGreen) * .769) + (((
                                  double)image[i][x].rgbtBlue) * .189));
            sepiaGreen[i][x] = ((((double)image[i][x].rgbtRed) * .349) + (((double)image[i][x].rgbtGreen) * .686) + (((
                                    double)image[i][x].rgbtBlue) * .168));
            sepiaBlue[i][x] = ((((double)image[i][x].rgbtRed) * .272) + (((double)image[i][x].rgbtGreen) * .534) + (((
                                   double)image[i][x].rgbtBlue) * .131));

            if (sepiaRed[i][x] > 255)
            {
                sepiaRed[i][x] = 255;
            }

            if (sepiaGreen[i][x] > 255)
            {
                sepiaGreen[i][x] = 255;
            }

            if (sepiaBlue[i][x] > 255)
            {
                sepiaBlue[i][x] = 255;
            }
            // rounding values
            image[i][x].rgbtRed = lround(sepiaRed[i][x]);
            image[i][x].rgbtGreen = lround(sepiaGreen[i][x]);
            image[i][x].rgbtBlue = lround(sepiaBlue[i][x]);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x < (width - x - 1))
            {
                RGBTRIPLE temp = image[i][x];
                image[i][x] = image[i][width - x - 1];
                image[i][width - x - 1] = temp;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temp array
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            temp[i][x] = image[i][x];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            //initialize every time values
            float sumred = 0;
            float sumgreen = 0;
            float sumblue = 0;
            int counter = 0;

            for (int s = -1; s < 2; s++)
            {
                // s looks for height 'neighbours'
                for (int t = -1; t < 2; t++)
                {
                    // t looks for width 'neighbours'
                    if ((i + s < 0) || (i + s >= height))
                    {
                        continue;
                    }

                    if ((x + t < 0) || (x + t >= width))
                    {
                        continue;
                    }

                    //condition to count only the pixel within the image
                    sumred = sumred + (float)temp[i + s][x + t].rgbtRed;
                    sumgreen = sumgreen + (float)temp[i + s][x + t].rgbtGreen;
                    sumblue = sumblue + (float)temp[i + s][x + t].rgbtBlue;
                    counter = counter + 1;
                }
            }
            // assigning values and rounding them
            image[i][x].rgbtRed = lround(sumred / counter);
            image[i][x].rgbtGreen = lround(sumgreen / counter);
            image[i][x].rgbtBlue = lround(sumblue / counter);
        }
    }
    return;
}

