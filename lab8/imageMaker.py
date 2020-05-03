import sys as System
from random import randint

fileName = System.argv[1]
width = int(System.argv[2])
height = int(System.argv[3])

file = open(fileName, "w+")

file.write("P2\n")
file.write(str(width))
file.write(" ")
file.write(str(height))
file.write("\n")

for i in range(height):
    for j in range(width):
        file.write(str(randint(0, 255)))
        file.write(" ")
    file.write("\n")

file.close()
