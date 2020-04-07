import random as r
import string

directoryName = "largeTests"
numberOfFiles = 20
numberOfLines = 1000
numberOfCharsPerLine = 100

for i in range(numberOfFiles):
    file = open(directoryName+"/file"+str(i)+".txt","w")
    for j in range(numberOfLines):
        letters = string.ascii_letters
        for z in range(numberOfCharsPerLine):
            file.write(r.choice(letters))
        file.write("\n")
    file.close()
