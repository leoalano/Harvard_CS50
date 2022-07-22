import cs50
import re


def main():
    # input from user
    input = cs50.get_string("Text: ")

    # calculate number of letters (no symbols)
    letters = count_letters(input)

    # count number of words
    words = len(input.split())

    # count number of sentences
    sentences = len(re.findall('[.?!]', input))

    # calculate grade of text (index)
    averagelet = (letters / words) * 100
    averageword = (sentences / words) * 100

    index = (0.0588 * averagelet) - (0.296 * averageword) - 15.8

    if (index >= 16):
        print("Grade 16+")
        
    elif (index < 1):
        print("Before Grade 1")
        
    else:
        print(f"Grade {int(round(index))}")

    print(letters)
    print(words)
    print(sentences)


# function to count letters
def count_letters(phrase):
    counter = 0
    for letter in phrase:
        if (letter.isalpha()):
            counter = counter + 1

    return counter


if __name__ == "__main__":
    main()