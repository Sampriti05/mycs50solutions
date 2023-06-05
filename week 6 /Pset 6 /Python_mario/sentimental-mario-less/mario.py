# TODO
from cs50 import get_int


def main():
    while True:
        n = get_int("Height: ")
        # as long as n is in the range of 1-8 inclusive the block will be printed
        # if it falls out of the range it will reprompt to get new height
        if n > 0 and n < 9:
            for i in range(1, n + 1):
                print(" " * (n-i) + "#" * i)
            break


main()

