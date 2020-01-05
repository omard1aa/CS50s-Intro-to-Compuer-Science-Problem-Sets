#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void check_visa(int arr[], int size);
void check_american_express(int arr[], int size);
void check_master_card(int arr[], int size);
bool check_credit_card(int arr[], int size);
void ReverseArray(long long arr[], int size);

int main(int argc, char *argv[])
{
    string credit_number;
    bool check = false;
    do
    {
        credit_number = get_string("Number: ");
        for (int i = 0; i < strlen(credit_number); i++)
        {
            if (isdigit(credit_number[i]))
            { 
                // Check wheather the input is valid or not
                check = true;
            }
            else
            {
                check = false;
                break;
            }
        }
    }
    while (check == false);
    const int LENGTH = strlen(credit_number);
    int credit_digits[LENGTH];
    for (int i = 0; i < LENGTH; i++)
    {
        credit_digits[i] = (int)credit_number[i] - 48;
    }
    const int LENGTH_2 = LENGTH / 2;
    int credit_digits_2[LENGTH_2];
    
    if (LENGTH == 13)
    {
        if (credit_digits[0] == 4)
        {
            check_visa(credit_digits, LENGTH);
        }
        else
        {
            printf("INVALID\n");
        } 
    }
    else if (LENGTH == 15)
    {
        if (credit_digits[0] == 3)
        {
            if (credit_digits[1] == 4 || credit_digits[1] == 7)
            {
                check_american_express(credit_digits, LENGTH);
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (LENGTH == 16)
    {
        if (credit_digits[0] == 5)
        {
            if (credit_digits[1] > 0 && credit_digits[1] < 6)
            {
                check_master_card(credit_digits, LENGTH);
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            if (credit_digits[0] == 4)
            {
                check_visa(credit_digits, LENGTH);
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

void check_visa(int arr[], int size)
{
    if (check_credit_card(arr, size))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
void check_american_express(int arr[], int size)
{
    if (check_credit_card(arr, size))
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
void check_master_card(int arr[], int size)
{
    if (check_credit_card(arr, size))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

bool check_credit_card(int my_arr[], int size)
{
    bool validate = false;
    const int SIZE = size;
    int end = size - 1;
    int arr[SIZE];
    
    // Saving original array into another array
    for (int i = 0; i < size; i++)
    {
        arr[i] = my_arr[i];
    }
    int sum = 0;
    
    // Multiply each other digit of the array by 2
    for (int i = end - 1; i >= 0; i -= 2)
    {
        arr[i] = arr[i] * 2;
        while (arr[i])
        {
            sum += arr[i] % 10;
            arr[i] /= 10;
        }
    }
    
    // Return array to its old original values:
    for (int i = 0; i < size; i++)
    {
        arr[i] = my_arr[i];
    }
    int sum_2 = 0;
    
    // Adding the sum of multiplied digits to the sum of the digits that weren't multiplued by 2(starting from the end):
    for (int i = end; i >= 0; i -= 2)
    {
        sum_2 += arr[i];
    }
    // check if last digit of the final sum is 0 or not
    int validate_digit = sum + sum_2;
    validate_digit = validate_digit % 10;
    if (validate_digit == 0)
    {
        validate = true;
    }
    return validate;
}
