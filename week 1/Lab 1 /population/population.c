#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int i;
    do
    {
        i = get_int("Start size: ");
    }
    while (i < 9);

    // TODO: Prompt for end size
    int e;
    do
    {
        e = get_int("End Size: ");
    }
    while (e < i);

    // TODO: Calculate number of years until we reach threshold
    int n = 0;
    while (i < e)
    {
        i = i + ((i / 3) - (i / 4));
        n++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", n);
}
