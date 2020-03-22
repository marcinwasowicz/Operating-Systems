import random as r
import sys as s

fileName = s.argv[1]
rows = int(s.argv[2])
columns  = int(s.argv[3])

file = open(fileName, "w+")
file.write(str(rows))
file.write(" ")
file.write(str(columns))
file.write("\n")

for i in range(rows):
    for j in range(columns):
        file.write(str(r.randint(1,5)))
        file.write(" ")
    file.write("\n")
file.close()