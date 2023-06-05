#include <cs50.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    printf("Text: %s\n", text);
    int l = count_letters(text);
    int w = count_words(text);
    int sen = count_sentences(text);
    float L = ((float)l / w) * 100;
    float S = ((float)sen / w) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    //to round off index to and integer -> storing it under grade
    int grade = (int)(index + 0.5);

    if (grade <= 16 && grade > 1)
    {
        printf("Grade %d\n", grade);
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }


}

int count_letters(string text)
{
    int i = 0;
    int s = strlen(text);
    int n = 0;
    for (i = 0; i < s; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            n++;
        }

    }
    //printf("letter: %i\n", n);
    return n; // so that the main function can have correct count
}



int count_words(string text)
{
    int m = 0;
    int i = 0;
    int s = strlen(text);
    for (i = 0; i < s; i++)
    {
        if (text[i] == ' ')// every space counting to a word
        {
            m++;
        }
    }
    //printf("words: %i\n",(m + 1) );
    return (m + 1); // m+1 as counting in spaces leaves out either the first word or the last word
    // so that the main function can have correct count
}

int count_sentences(string text)
{
    int o = 0;
    int i = 0;
    int s = strlen(text);
    for (i = 0; i < s; i++)
    {
        if (text[i] == '!' || text[i] == '.' || text[i] == '?') // considering the presence of .,? being the end of a sentence
        {
            o++;
        }
    }
    //printf("sentences: %i\n",o );
    return o; // so that the main function can have correct count
}
