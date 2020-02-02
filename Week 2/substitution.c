#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string encrypt(string plaintext, char *key);
bool uniqueCharacters(string str);

int main(int argc, string argv[])
{
    if (argv[1] == NULL || atoi(argv[1]) != 0) // User doesn't cooperate
    {
        printf("Usage: ./caesar keys\n");
        return 1;
    }
    if (argv[2] != NULL) // User inputs more than 1 parameter (1 key)
    {
        printf("Usage: ./caesar keyss\n");
        return 1;
    }
    int size = strlen(argv[1]);
    if (size != 26)
    {
        printf("Key must be 26 alphabet characters\n");
        return 1;
    }
    else if (!uniqueCharacters(argv[1]))
    {
        printf("Key must be unique 26 alphabet characters\n");
        return 1;
    }
    for (int i = 0; i < size; i++)
    {
        if (isdigit(argv[1][i]))
        {
            printf("Key must be 26 alphabet characters\n");
            return 1;
        }
    }
    char *k = argv[1];
    string plaintext = get_string("plaintext: ");
    string ciphertext = encrypt(plaintext, k);
    printf("ciphertext: %s\n", ciphertext);
}

// Encrypt a string with specific key
string encrypt(string plaintext, char *key)
{
    char ciphertext[strlen(plaintext)];
    string cipher = plaintext;
    int index = 0;
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if ((int)plaintext[i] >= 65 && (int)plaintext[i] <= 90) // Check for Capital letter
        {
            index = (int)plaintext[i] - 65;
            ciphertext[i] = toupper(key[index]);
        }
        else if ((int)plaintext[i] >= 97 && (int)plaintext[i] <= 122) // check for Small letter
        {
            index = (int)plaintext[i] - 97;
            ciphertext[i] = tolower(key[index]);
        }
        else
        {
            ciphertext[i] = plaintext[i];
            continue;
        }
    }
    for (int i = 0; i < strlen(plaintext); i++)
    {
        cipher[i] = ciphertext[i];
    }
    return cipher;
}


bool uniqueCharacters(string str)
{

    // If at any time we encounter 2
    // same characters, return false
    for (int i = 0; i < strlen(str) - 1; i++)
    {
        for (int j = i + 1; j < strlen(str); j++)
        {
            if (str[i] == str[j])
            {
                return false;
            }
        }
    }
    // If no duplicate characters encountered,
    // return true
    return true;
}

