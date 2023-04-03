import math, time

last = 10000
found = 4     # we already know 2, 3, 5, 7
print('Prime numbers to {}'.format(last))

start = time.monotonic()

# code 1 - 45.685 seconds for 10000 on Feather M0 - 17.3x slower
"""
for number in range(11, last, 2): # we already know 2, 3, 5, 7
    prime = True
    for divider in range(2, int(math.sqrt(number))+1, 1):
        if number/divider == int(number/divider):
            prime = False
    if prime:
        #print(',', number, end='')
        found += 1
"""

# code 2 - 15.555 seconds for 10000 on Feather M0 - 5.88x slower
"""
for number in range(11, last, 2):
    prime = True
    for divider in range(2, int(math.sqrt(number))+1, 1):
        if number % divider == 0:
            prime = False

    if prime:
        #print(',', number, end='')
        found += 1
"""

# code 3 - 2.645 seconds for 10000 on Feather M0

for number in range(11, last, 2):
    prime = True
    if number % 3 == 0:
            prime = False
    elif number % 5 == 0:
            prime = False
    elif number % 7 == 0:
            prime = False
    if prime:
        for divider in range(11, int(math.sqrt(number))+1, 2):
            if number % divider == 0:
                prime = False
                break
    if prime:
        found += 1

end = time.monotonic()
print(f"This took: {(end - start)} seconds.")
print(f"I found {found} prime numbers.")
