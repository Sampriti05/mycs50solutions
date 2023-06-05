#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO
    //asking user for message
    string message = get_string("Message: ");
    int n = strlen(message);
    int i;

    for (i = 0; i < n; i++)
    {
        char a = message[i];
        int v = a;
        int quotient = v;
        int q = 0;
        int rem[8];//max 8 bits

        do
        {
            rem[q] = v % 2;
            //printf("%d", remainder);
            quotient = v / 2;
            v = quotient; //updates the value of v
            q++;
        }
        while (quotient > 0);

        //adding 0 in the front to make up to 8bits
        while (q < 8)
        {
            rem[q] = 0;
            q++;
        }

        for (int r = 7 ; r >= 0; r--)
        {
            print_bulb(rem[r]);
        }

        printf("\n");

    }
    return 0;

}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
