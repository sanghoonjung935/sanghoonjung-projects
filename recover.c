#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Defining block_size as 512, for this example as there are 512 bytes per block of data
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Accept only a single command-line arg
    if (argc != 2)
    {
        printf("Usage: ./recover FILE/n");
        return 1;
    }

    // card points to FILE structure
    FILE *card = fopen(argv[1], "r");

    // Checking if file opened correctly
    if (card == NULL)
    {
        printf("Error opening file\n");
        return 2;
    }

    // Creating a buffer to store a "block" of information from
    // the card.raw file. One block is 512 bytes.
    // Initializing buffer of 512 elements, each of type uint8_t
    uint8_t buffer[BLOCK_SIZE];

    // Initializing variable jpg_number to keep track of how many jpgs have been printed so far
    int jpg_number = 0;

    // Declaring jpeg to be used later
    FILE *jpeg = NULL;

    // Making array called output[] to store output file name
    char output[8];

    // Want to keep reading data from the memory card if there is data
    // We know there is data if fread() returns the BLOCK_SIZE.
    // We will know there is no more data if fread() returns 0, indicating
    // that there are 0 bytes left to read.
    // Implementing while loop to keep reading if there is data left
    // Buffer is temporary place to store data, 1 is the size in bytes we
    // will read at a time, BLOCK_SIZE is the total number of (1 byte)s we
    // will read, card is the file we will read from.
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // Checking for the start of a jpeg file. Using bitwise and operator (&) to see if the fourth byte of the buffer
        // is one of the 16 valid bytes that signifies the start of a jpeg file.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If we found the first jpeg file, we need to treat it differently, since we don't need to close the file:
            if (jpg_number == 0)
            {
                // Using sprintf() to update output file name.
                sprintf(output, "%03i.jpg", jpg_number);

                // Increasing jpg_number by 1
                jpg_number++;

                jpeg = fopen(output, "w");
                // Checking if file opened correctly
                if (jpeg == NULL)
                {
                    printf("Error opening file\n");
                    return 1;
                }

                // Writing to the output file the contents of the 512-byte buffer
                fwrite(buffer, 1, BLOCK_SIZE, jpeg);
            }

            // If it is not the first jpeg file found:
            else
            {
                // Closing file and resetting jpeg file to NULL such that it does not point to an invalid location
                // If not the first jpeg, need to close previous file.
                fclose(jpeg);
                jpeg = NULL;

                // Then, repeat the same process of opening new file and writing to it. sprintf() ensures new file name
                // in accordance with jpg_number.
                sprintf(output, "%03i.jpg", jpg_number);

                jpeg = fopen(output, "w");
                // Checking if file opened correctly
                if (jpeg == NULL)
                {
                    printf("Error opening file\n");
                    return 1;
                }

                // Writing to the output file the contents of the 512-byte buffer
                fwrite(buffer, 1, BLOCK_SIZE, jpeg);
                //
                jpg_number++;
            }
        }

        // Using this condition because it indicates a jpeg header has been found; we don't want to read before the start of the
        // first jpeg
        else if (jpg_number > 0)
        {
            // We want to continue writing to the output file
            // Writing to the output file the contents of the 512-byte buffer
            fwrite(buffer, 1, BLOCK_SIZE, jpeg);
        }
    }

    // Closing the last jpeg file and setting pointer jpeg to NULL
    fclose(jpeg);
    jpeg = NULL;

    // Doing the same for pointer card
    fclose(card);
    card = NULL;
}