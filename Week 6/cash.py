from cs50 import get_float


def main():
    number = 0
    while (number <= 0):
        number = get_float("Enter your cash: ")
    print(cash(number))


def cash(number):
    coins = 0
    while number >= 1:
        number -= 1
        coins += 4
    while number >= 0.25:
        number -= 0.25
        number = round(number, 2)
        coins += 1
    while number >= 0.1:
        number -= 0.1
        number = round(number, 2)
        coins += 1
    while number >= .05:
        number -= 0.05
        number = round(number, 2)
        coins += 1
    while number >= 0.01:
        number -= 0.01
        number = round(number, 2)
        coins += 1
    return coins


main()
