#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    //ensure the input of exactly one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //opening memory card
    char *inputfile = argv[1];
    FILE *inputpointer = fopen(inputfile, "r");
    if (inputpointer == NULL)
    {
        printf("Could not open the file %s.\n", inputfile);
        return 2;
    }



    //Making an array to store block of bytes , and count to keep track of number of JPEG
    BYTE block[BLOCK_SIZE];
    int count = 0;
    FILE *imgpointer = NULL;
    char filename[8];

    while (fread(&block, 512, 1, inputpointer) == 1)
    {
        //finding the start of a JPEH
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            //JPEG found is not the first -> close previous file
            if (count != 0)
            {
                fclose(imgpointer);
            }
            //JPEG found be it first or any place
            sprintf(filename, "%03i.jpg", count);
            imgpointer = fopen(filename, "w");
            count++;

        }
        //writing in file once JPEG found
        if (!(count == 0))
        {
            fwrite(&block, 512, 1, imgpointer);
        }

    }
    fclose(inputpointer);
    fclose(imgpointer);

    return 0;
}



