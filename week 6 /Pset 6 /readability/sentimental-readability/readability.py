# TODO
from cs50 import get_string

text = get_string("Text: ")
Letters = 0
Words = 1
Sentences = 0
for i in range(len(text)):
    if text[i].isalpha():
        Letters += 1
    elif text[i].isspace():
        Words += 1
    elif text[i] == '.' or text[i] == '!' or text[i] == '?':
        Sentences += 1

l = Letters / Words * 100
s = Sentences / Words * 100
index = 0.0588 * l - 0.29 * s - 15.8
index = round(index)
if index > 16:
    print("Grade 16+")
elif index > 1 and index < 17:
    print(f"Grade {index}")
else:
    print("Before Grade 1")