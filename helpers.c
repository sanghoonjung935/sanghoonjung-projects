#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Looping for every pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating the average of the RGB values for the grayscale image. Dividing by 3.0 instead of
            // 3 to minimize rounding error. Calling round() from <math.h> to round the float, since RGB
            // values are integers, not floats.
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Updating each pixel in the image to have its RGB values all be the same; in this way, we have
            // successfully updated each pixel into grayscale.
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia

// Declaring function lesser() prototype
int lesser(int number1, int number2);


void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Similarly looping for every pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating sepiaRed, sepiaGreen, and sepiaBlue as per the formula. Using round() function again from
            // <math.h> to round RGB values to nearest integer.
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // Updating RGB values of pixels to the lesser number of sepiaRed/Green/Blue and 255, as to not go over the
            // maximum value for RGB.
            image[i][j].rgbtRed = lesser(sepiaRed, 255);
            image[i][j].rgbtGreen = lesser(sepiaGreen, 255);
            image[i][j].rgbtBlue = lesser(sepiaBlue, 255);
        }
    }
    return;
}

// Implementing function lesser() to return the smaller of two integers
int lesser(int number1, int number2)
{
    if (number1 > number2)
    {
        // If number1 is larger than number2, return number2
        return number2;
    }

    else if (number2 > number1)
    {
        // If number2 is larger than number1, return number1
        return number1;
    }

    else
    {
        // Else implies numbers are equal, in which case return number1
        return number1;
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Looping over every row, but only the left half pixels in a row.
    // This is because for every pixel, I can swap the RGB value with
    // that of the opposite pixel in the row. If there are an odd number
    // of pixels in a row, this is fine because if I say something like
    // "iterate over j < width / 2", this will cut off the decimal, and
    // essentially not do anything to the middle pixel. This is what we want.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swapping each pixel with the "opposite/reflective" pixel.
            // Having a template tmpRed to temporarily hold original pixel's rgbtRed
            // value, then assigning the original pixel's rgbtRed value to the opposite
            // pixel's rgbtRed value. Opposite pixel is image[i][width - j - 1], because
            // it is on the same row, move "left" if you will by j, and subtract 1 to
            // account for indexing.
            int tmpRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtRed = tmpRed;

            // Repeat the process for Green and Blue
            int tmpGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtGreen = tmpGreen;

            int tmpBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtBlue = tmpBlue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Creating copy of image, such that blur effect of one pixel does not affect blur effect of others.
    // Creating copy and calculating average of neighboring RGB values of each pixel in copy image, then
    // replacing this "blurred" pixel for each pixel in original image gets around the issue.

    // Initializing copy 2D - array, copy[][]
    RGBTRIPLE copy[height][width];
    // Copying every pixel from image[][] to copy[][]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Now, will calculate average RGB value of all neighboring pixels for each pixel in copy[][],
    // then replace the RGB values of each pixel in the original image with these "blurred" values.

    // Going through each pixel to "blur" it
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Re-initializing these variables to 0 for each iteration (each pixel). totalRed/Blue/Green keeps track of
            // sum of RGB values for each valid neighbor pixel to be used in the calculation for average R/G/B value
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;
            // Re-initializing valid_pixels to 0, will go up each time a valid pixel is counted, to be used in average calculation.
            int valid_pixels = 0;

            // For each pixel, iterating through each neighbor pixel & center pixel
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // Checking if the neighbor pixel is valid
                    if (i + di >= 0 && j + dj >= 0 && i + di < height && j + dj < width)
                    {
                        // If neighbor pixel is valid, add its R/G/B value to the total R/G/B.
                        totalRed += copy[i+di][j+dj].rgbtRed;
                        totalGreen += copy[i+di][j+dj].rgbtGreen;
                        totalBlue += copy[i+di][j+dj].rgbtBlue;
                        // If neighbor pixel is valid, increase number of valid_pixels by 1.
                        valid_pixels ++;
                    }
                }
            }
            // Again calling round() from <math.h>, and dividing by float (valid_pixels) to avoid rounding error
            // Calculating average R/G/B for each pixel
            int averageRed = round(totalRed / (float) valid_pixels);
            int averageGreen = round(totalGreen / (float) valid_pixels);
            int averageBlue = round(totalBlue / (float) valid_pixels);


            // Replacing each pixel in original image, as opposed to copy for reasons discussed previously
            image[i][j].rgbtRed = averageRed;
            image[i][j].rgbtGreen = averageGreen;
            image[i][j].rgbtBlue = averageBlue;

        }
    }

    return;
}
