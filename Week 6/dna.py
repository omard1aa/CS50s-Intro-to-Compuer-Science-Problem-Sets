import sys, csv


def main(csv_file, text_file):
    if csv_file[-3:] != "csv":
        print("Usage: python dna.py data.csv sequence.txt")
        return
    if text_file[-3:] != "txt":
        print("Usage: python dna.py data.csv sequence.txt")
        return
    # Problem implementation
    csv_data = read_csv(csv_file)
    sequence = read_text(text_file)
    str1 = ""
    answer = ""
    count = 0
    max_count = 0
    brk = False
    match = False
   
    for i in range((len(csv_data[0]) - 1)): # Loop throw STRs in csv file
        STR = csv_data[0][i + 1]
        inc = 0
        max_count = 0
        count = 0
        for j in range(0, ((len(sequence) - len(STR)) + 1)): # Loop throw sequence
            current_STR = sequence[inc: inc + len(STR)]
            if len(STR) != len(STR):
                #print("continue")
                inc += 1
                continue
            #print(STR, current_STR, j, "HI")
            if STR == current_STR:
                inc += len(STR)
                match = True
                count += 1
                #print("True", count, max_count, STR, current_STR, inc)
            else:
                inc += 1
                if count > max_count:
                    max_count = count
                #print("False",count, max_count, inc)
                count = 0
        inc = 0
        #print("max count", max_count)
        if max_count == 1:
            max_count = 0
            #print("max count 0", max_count)
        for k in range(len(csv_data) - 1): # Check for STRs repeats in csv file
            #print(k, max_count, csv_data[k + 1][i + 1], "i:", i)
            if max_count == int(csv_data[k + 1][i + 1]):
                brk = True
                answer = csv_data[k + 1][0]
                break
        if brk == True:
            break
    if answer == "":
        answer = "No match"
    print(f"{answer}")


# Read data from csv file, load it to a list and return it
def read_csv(csv_file):
    csv_data = []
    with open(f'dna/{csv_file}','rt')as f:
        csv_read = csv.reader(f)
        for row in csv_read:
            csv_data.append(row)
            #print(row)
    #print(csv_data)
    return csv_data


# Read data from text file, load it to a string and return it
def read_text(text_file):
    sequence = ""
    file = open(f"dna/{text_file}","r")
    for line in file:
        sequence += line
    #print(sequence)
    return sequence


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python dna.py data.csv sequence.txt")
    else:
        csv_file = sys.argv[1]
        text_file = sys.argv[2]
        main(csv_file, text_file)
