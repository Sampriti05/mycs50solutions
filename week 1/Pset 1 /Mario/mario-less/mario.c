#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    //making each row
    for (int i = 0; i < n; i++)
    {
        //to create a space
        for (int j = n - i; j > 1; j--)
        {
            printf(" ");
        }
        //to create a block
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        //to move on to next line
        printf("\n");
    }
}