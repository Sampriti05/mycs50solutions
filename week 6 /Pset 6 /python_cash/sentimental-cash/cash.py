# TODO
from cs50 import get_float


def get_cents():
    while True:
        # Ask how many dollars the customer is owed
        dollars = get_float("Change owed: ")
        cents = round(dollars * 100)
        if cents >= 0:
            return cents


def calculate_quarters(cents):
    quarters = cents // 25
    return quarters


def calculate_dimes(cents):
    dimes = cents // 10
    return dimes


def calculate_nickels(cents):
    nickels = cents // 5
    return nickels


def calculate_pennies(cents):
    pennies = cents // 1
    return pennies


def main():
    cents = get_cents()

# Calculate the number of quarters to give the customer
    quarters = calculate_quarters(cents)
    cents = cents - int(quarters) * 25

# Calculate the number of dimes to give the customer
    dimes = calculate_dimes(cents)
    cents = cents - int(dimes) * 10

# Calculate the number of nickels to give the customer
    nickels = calculate_nickels(cents)
    cents = cents - int(nickels) * 5

# Calculate the number of pennies to give the customer
    pennies = calculate_pennies(cents)
    cents = cents - int(pennies) * 1

# sum coins
    coins = quarters + dimes + nickels + pennies

    print(f"{coins}\n")


main()

