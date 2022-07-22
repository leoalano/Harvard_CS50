import cs50

# user input
input = cs50.get_int("Number: ")

# put numbers in a list called digits
digits = []

y = 1
while (y <= 1000000000000000):
    x = (input/y) % 10
    x = int(x)
    digits.append(x)
    y *= 10

# multiply by 2 every other number and and digits if > 9,
# according to luhn's algorithm
for multiple in range(1, 17, 2):
    digits[multiple] = digits[multiple] * 2
    if (digits[multiple] > 9):
        finaldigit = digits[multiple] % 10
        firstdigit = (digits[multiple] / 10) % 10
        digits[multiple] = int(finaldigit + firstdigit)
        
# sum is the luhn's algorithm of verification of a card
luhnsum = (sum(digits))

# check for card lenght and starting digits

if (luhnsum % 10 != 0):
    print("INVALID")

elif ((input >= 340000000000000 and input < 350000000000000) or (input >= 370000000000000 and input < 380000000000000)):
    print("AMEX")

elif ((input >= 4000000000000000 and input < 5000000000000000) or (input >= 4000000000000 and input < 5000000000000)):
    print("VISA")

elif (input >= 5100000000000000 and input < 5600000000000000):
    print("MASTERCARD")

else:
    print("INVALID")