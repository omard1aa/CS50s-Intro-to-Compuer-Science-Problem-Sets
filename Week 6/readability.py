from cs50 import get_string


def main():
    text = get_string("Text: ")
    grade = get_grade(text)
    print(grade)


def get_grade(text):
    W = calc_words(text)
    L = calc_letters(text) / W * 100
    S = calc_sentences(text) / W * 100
    grade = 0.0588 * L - 0.296 * S - 15.8
    if grade < 1:
        return "Before Grade 1"
    elif grade >= 16:
        return "Grade 16+"
    else:
        return f"Grade {int(round(grade))}"


def calc_words(text):
    words = 0
    calc = False
    for letter in text:
        if ord(letter) >= 65 and ord(letter) <= 122:
            calc = True
        if letter == " " or letter == "?" or letter == "!" or letter == ".":
            if calc:
                words += 1
            calc = False
    return words


def calc_letters(text):
    letters = 0
    words = calc_words(text)
    for letter in text:
        if ord(letter) >= 65 and ord(letter) <= 122:
            letters += 1
    return letters


def calc_sentences(text):
    sentences = 0
    words = calc_words(text)
    for letter in text:
        if letter == "." or letter == "!" or letter == "?":
            sentences += 1
    return sentences


if __name__ == "__main__":
    main()
