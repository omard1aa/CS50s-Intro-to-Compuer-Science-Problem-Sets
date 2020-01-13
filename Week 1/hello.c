#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main()
{
    string name = get_string("Name: ");
    printf("Hello, %s\n", name);
}
