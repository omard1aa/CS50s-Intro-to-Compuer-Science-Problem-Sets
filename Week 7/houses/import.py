from cs50 import SQL
import sys
import csv
db = SQL("sqlite:///students.db")

first_names = []
middle_names = []
last_names = []
birth_years = []
houses = []


def main(csvfile):
    with open(f"{csvfile}", "r")as csv_file:
        reader = csv.DictReader(csv_file)
        for row in reader:
            houses.append(row['house'])
            birth_years.append(row['birth'])
            if len(row['name'].split(' ')) == 3:
                first_names.append(row['name'].split(' ')[0])
                middle_names.append(row['name'].split(' ')[1])
                last_names.append(row['name'].split(' ')[2])
            elif len(row['name'].split(' ')) == 2:
                first_names.append(row['name'].split(' ')[0])
                middle_names.append(None)
                last_names.append(row['name'].split(' ')[1])
    for index in range(len(first_names)):
        db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                   first_names[index], middle_names[index], last_names[index], houses[index], birth_years[index])


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python roster.py CSV file name")
    else:
        csvfile = sys.argv[1]
        main(csvfile)
