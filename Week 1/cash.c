#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_change(int cash);

int main(void)
{
    float cash;
    int counter = 0;
    do
    {
        cash = get_float("Change owed: ");
    }
    while (cash <= 0);
    int cents = round(cash * 100);
    counter = get_change(cents);
    printf("%i\n", counter);
}

int get_change(int cash)
{
    int counter = 0;
    while (cash > 0)
    {
        if (cash >= 25)
        {
            cash -= 25;
            counter++;
        }
        else if (cash < 25 && cash >= 10)
        {
            cash -= 10;
            counter++;
        }
        else if (cash < 10 && cash >= 5)
        {
            cash -= 5;
            counter++;
        }
        else if (cash < 5)
        {
            cash--;
            counter++;
        }
    }
    return counter;
}
