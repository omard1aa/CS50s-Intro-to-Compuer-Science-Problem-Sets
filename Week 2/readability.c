#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void get_grade(string text);
float calc_words(char *text);
float calc_letters(string text);
float calc_sentences(string text);

int main()
{
    char *text = get_string("Text: ");
    get_grade(text);
}

void get_grade(string text)
{
    int W = calc_words(text);
    float L = calc_letters(text) / W * 100; // Calculate number of letters in the text
    float S = calc_sentences(text) / W * 100; // Calculate number of words in the text
    float fgrade = 0.0588 * L - 0.296 * S - 15.8; // Calculate Coleman-Liau index
    int grade = round(fgrade); // Convert float grade to integer and round it to nearest decimal

    // Determine the readability grade of a text according to Coleman-Liau index equaion
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// Calculate number of words of a text function
float calc_words(char *text)
{
    int words = 0;
    bool calc = false;
    int convert;
    const char *mytext = text;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((int)text[i] >= 65 && (int)text[i] <= 122) // Check if the character is a alphabet or not
        {
            calc = true;
        }
        if (text[i] == ' ' || text[i] == '?' || text[i] == '!' || text[i] == '.')
        {
            // If one condition is true then it is the end of word
            if (calc) // that condition checks the repeat of dots or '!' and if it was two delimeters don't increment like '. '
            {
                words++; // Then increment number of words by one
            }
            calc = false;
        }
    }
    return words;
}

// Calculate number of letters of a text function
float calc_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // If the character is an alphabet then increment the number of letters
        if (((int)text[i] >= 65 && (int)text[i] <= 90) || ((int)text[i] >= 97 && (int)text[i] <= 122))
        {
            letters++;
        }
    }
    return letters;
}

// Calculate number of sentences of a text function
float calc_sentences(char *text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

