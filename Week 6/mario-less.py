def main():
    number = input("Enter number: ")
    while int(number) > 8 or int(number) <= 0:
        number = input("Enter number: ")
    draw(int(number))


def draw(number):
    n = number - 1
    shape = ""
    for i in range(number):
        for j in range(n):
            shape += " "
        for k in range(number - n):
            shape += "#"
        print(shape)
        shape = ""
        n -= 1


main()

