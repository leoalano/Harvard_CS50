import csv
import cs50
import sys


def main():

    # Ensure correct usage
    if (len(sys.argv) != 3):
        sys.exit("Usage: python dna.py CSV.FILE TXT.FILE")

    # define data structures
    strs = []
    strsresults = []

    # open CSV file
    with open(sys.argv[1]) as csvfile:
        readercsv = csv.reader(csvfile)
        for row in readercsv:
            strs.append(row)

    # open TXT file
    with open(sys.argv[2]) as textfile:
        readertxt = textfile.read()

    # loop through all STR's to find longest run number and add them to a list
    strsresults.append("name")
    for i in range(1, len(strs[0])):
        strsresults.append(getlongest_strs(strs[0][i], readertxt))

    # MATCHING longest runs of all STR's in txt file with person in CSV. If so, print name, othw 'No Match'.
    for x in range(1, len(strs)):
        counter = 0
        for i in range(1, len(strs[x])):
            if (int(strsresults[i]) == int(strs[x][i])):
                counter += 1
                if (counter == len(strs[x]) - 1):
                    print(strs[x][0])
                    return 0

    print("No match")




# FUNCTION to get longest run of STR's located in the CSV file (first line)
def getlongest_strs(listofstrs, txt):
    count = 1
    cursor2 = 0
    cursor1 = 0
    longestrun = 0

    # initial find before looping
    cursor1 = txt.find(f"{listofstrs}", cursor2)

    # -1 is the return of find function in case it doens't find
    # this is a loop to find longest run
    while (cursor1 != -1):
        cursor2 = txt.find(f"{listofstrs}", (cursor1 + len(listofstrs)))
        if (cursor2 == cursor1 + len(listofstrs)):
            count = count + 1
            cursor1 = cursor2

        elif (longestrun < count):
            longestrun = count
            count = 1
            cursor1 = cursor2

        else:
            count = 1
            cursor1 = cursor2

    return longestrun


if __name__ == "__main__":
    main()