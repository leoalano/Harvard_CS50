//correct

#include "helpers.h"
#include <stdio.h>
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average[height][width];
    printf("%i\n", image[0][0].rgbtRed);
    //find average
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            average[i][x] = ((float)image[i][x].rgbtRed + (float)image[i][x].rgbtGreen + (float)image[i][x].rgbtBlue) / 3;

            image[i][x].rgbtRed = lround(average[i][x]);
            image[i][x].rgbtGreen = lround(average[i][x]);
            image[i][x].rgbtBlue = lround(average[i][x]);
        }
    }
    printf("%i\n", image[0][0].rgbtRed);

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
            sepiaRed[i][x] = ((((double)image[i][x].rgbtRed) * .393) + (((double)image[i][x].rgbtGreen) * .769) + (((double)image[i][x].rgbtBlue) * .189));
            sepiaGreen[i][x] = ((((double)image[i][x].rgbtRed) * .349) + (((double)image[i][x].rgbtGreen) * .686) + (((double)image[i][x].rgbtBlue) * .168));
            sepiaBlue[i][x] = ((((double)image[i][x].rgbtRed) * .272) + (((double)image[i][x].rgbtGreen) * .534) + (((double)image[i][x].rgbtBlue) * .131));
            
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
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}




//incorrect

#include "helpers.h"
#include <stdio.h>
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average[height][width];
    printf("%i\n", image[0][0].rgbtRed);
    //find average
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            average[i][x] = ((float)image[i][x].rgbtRed + (float)image[i][x].rgbtGreen + (float)image[i][x].rgbtBlue) / 3;

            image[i][x].rgbtRed = lround(average[i][x]);
            image[i][x].rgbtGreen = lround(average[i][x]);
            image[i][x].rgbtBlue = lround(average[i][x]);
        }
    }
    printf("%i\n", image[0][0].rgbtRed);

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
            sepiaRed[i][x] = ((((double)image[i][x].rgbtRed) * .393) + (((double)image[i][x].rgbtGreen) * .769) + (((double)image[i][x].rgbtBlue) * .189));
            sepiaGreen[i][x] = ((((double)image[i][x].rgbtRed) * .349) + (((double)image[i][x].rgbtGreen) * .686) + (((double)image[i][x].rgbtBlue) * .168));
            sepiaBlue[i][x] = ((((double)image[i][x].rgbtRed) * .272) + (((double)image[i][x].rgbtGreen) * .534) + (((double)image[i][x].rgbtBlue) * .131));

            image[i][x].rgbtRed = lround(sepiaRed[i][x]);
            image[i][x].rgbtGreen = lround(sepiaGreen[i][x]);
            image[i][x].rgbtBlue = lround(sepiaBlue[i][x]);

            if (image[i][x].rgbtRed > 255)
            {
                image[i][x].rgbtRed = 255;
            }

            if (image[i][x].rgbtGreen > 255)
            {
                image[i][x].rgbtGreen = 255;
            }

            if (image[i][x].rgbtBlue > 255)
            {
                image[i][x].rgbtBlue = 255;
            }
        }
    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
