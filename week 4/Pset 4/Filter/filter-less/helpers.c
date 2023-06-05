#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            float ave = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0;
            image[i][j].rgbtRed = round(ave);
            image[i][j].rgbtBlue = round(ave);
            image[i][j].rgbtGreen = round(ave);
        }
    }


    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            //using given algorithm
            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            //ensures the max is 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtBlue = round(sepiaBlue);
            image[i][j].rgbtGreen = round(sepiaGreen);

        }
    }
    return;
}

// Reflect image horizontally

//function to swap the pixel using a temp variable
void swap(RGBTRIPLE *pixel1, RGBTRIPLE *pixel2)
{
    RGBTRIPLE temp = *pixel1;
    *pixel1 = *pixel2;
    *pixel2 = temp;
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int n = width / 2; //centre column remains still -> used as a reference point 
    int i = 0;
    int j = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < n; j++)
        {
            //using the swap funciton above
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    // for each pixel in the image to blur
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            float red = 0;
            float blue = 0;
            float green = 0;
            float counter = 0;

            //for each pixel within the 3x3 table of the pixel
            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    //ensures corner pixels are filtered
                    if (i + a < 0 || i + a > height - 1)
                    {
                        continue;
                    }

                    if (j + b < 0 || j + b > width - 1)
                    {
                        continue;
                    }
                    red += image[a + i][b + j].rgbtRed;
                    blue += image[a + i][b + j].rgbtBlue;
                    green += image[a + i][b + j].rgbtGreen;
                    counter++;

                }
            }
            temp[i][j].rgbtRed = round(red / counter);
            temp[i][j].rgbtBlue = round(blue / counter);
            temp[i][j].rgbtGreen = round(green / counter);
        }

    }
// a seperate loop to replace each pixel with the blurred pixel -> to avoid overlap of blurred pixel in the above function
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
        }
    }


    return;
}
