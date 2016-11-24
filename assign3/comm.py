#!/usr/bin/python

import sys, locale
from optparse import OptionParser

def main():
    info_msg = """%prog [-1] [-2] [-3] [-u] in1 in2

    compares the lines of two files and outputs those
    unique to in1 in the first column, lines unique to 
    in2 in the second column and lines in both in the 
    third column.

    use -u when the files are unsorted."""

    parser = OptionParser(usage=info_msg)
    parser.add_option("-1", 
                    action="store_false", dest="printFile1", default=True)

    parser.add_option("-2", 
                    action="store_false", dest="printFile2", default=True)

    parser.add_option("-3", 
                    action="store_false", dest="printBoth", default=True)

    parser.add_option("-u",
                    action="store_true", dest="unsorted", default=False)

    global file1
    global file2
    global opts
    opts, args = parser.parse_args()

    if len(args) != 2:
        parser.error("Incorrect number of input files. Must be 2.")
        return 1

    in1 = args[0]
    in2 = args[1]

    try:
        if in1 == '-':
            text1 = sys.stdin
        else:
            text1 = open(in1, 'r')

        if in2 == '-':
            text2 = sys.stdin
        else:
            text2 = open(in2, 'r')

        file1 = text1.read().splitlines()
        file2 = text2.read().splitlines()
        text1.close()
        text2.close()

    except IOError as e:
        parser.error("I/O error({0}): {1}". format(e.errno, e.strerror))


    global col2buffer
    global col3buffer
    col2buffer = ""
    col3buffer = ""

    if opts.printFile1:
        col2buffer += "\t"
        col3buffer += "\t"

    if opts.printFile2:
        col3buffer += "\t"

    if opts.unsorted:
        printUnsorted()
    else:
        printSorted()

def printSorted():
    iter1 = 0
    iter2 = 0

    #compare each line in the files in order, then
    #print the line to the correct column
    while iter1 < len(file1) and iter2 < len(file2):
        onebefore2result = locale.strcoll(file1[iter1], file2[iter2])
        if onebefore2result ==0:
            if opts.printBoth:
                print(col3buffer + file1[iter1])
            iter1 += 1
            iter2 += 1
        
        elif onebefore2result < 0:
            if opts.printFile1:
                print(file1[iter1])
            iter1 += 1

        else:
            if opts.printFile2:
                print(col2buffer + file2[iter2])
            iter2 += 1

    # if any lines are left, print them til the end
    if iter1 < len(file1) and opts.printFile1:
        for i in range(iter1, len(file1)):
            print(file1[i])

    if iter2 < len(file2) and opts.printFile2:
        for i in range(iter2, len(file2)):
            print(file2[i])

def printUnsorted():
    for line in file1:
        if line in file2:
            file2.remove(line)
            if opts.printBoth:
                print(col3buffer + line)

        elif opts.printFile1:
            print(line)

    if opts.printFile2:
        for line in file2:
            print(col2buffer + line)

if __name__ == "__main__":
    main()
