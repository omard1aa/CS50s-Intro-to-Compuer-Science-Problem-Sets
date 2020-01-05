from cs50 import get_string


# main method
def main():
    credit_card_number = get_string("Enter a credit card number:")
    credit(credit_card_number)


def credit(number):  # Program main logic
    type = ""
    check(number)
    if check:
        type = detect(number)   # identify which type the card is
        print(type)
    else:
        type = "INVALID"
    return type


# Check whether the number is legit and it's formula in Hans Peter Luhn algorithm
def check(number):
    sum = 0
    count = 0
    str1 = ""
    length = int(len(number))
    for i in range(length - 2, -1, -2):
        count = int(number[i]) * 2   # multiple each other digit by 2
        if len(str(count)) > 1:
            for i in range(len(str(count))):  # sum the number that container more than 1 digit
                str1 = str(count)
                sum += int(str1[i])
        else:
            sum += count
    for j in range(length - 1, -1, -2):  # sum other digits
        sum += int(number[j])
    if sum % 10 == 0:
        return True
    else:
        return False


# Identify which type the card is
def detect(number):
    type = ""
    if len(number) == 13:
        type = "VISA"
    elif len(number) == 15:
        if int(number[0]) == 3:
            if int(number[1]) == 4 or int(number[1]) == 7:
                type = "AMEX"
            else:
                type = "INVALID"
        else:
            type = "INVALID"
    elif len(number) == 16:
        if int(number[0]) == 4:
            type = "VISA"
        elif int(number[0]) == 5:
            if int(number[1]) >= 1 and int(number[1]) <= 5:
                type = "MASTERCARD"
        else:
            type = "INVALID"
    else:
        type = "INVALID"
    return type


main()
