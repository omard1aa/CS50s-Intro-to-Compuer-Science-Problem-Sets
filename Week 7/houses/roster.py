from cs50 import SQL
import sys

db = SQL("sqlite:///students.db")
full_result = []


def main(house_name):
    result = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last;", house_name);
    for i in range(len(result)):
        if result[i]['middle'] is None:
            full_result.append(result[i]['first'] + " " + result[i]['last']
                               + ", born " + str(result[i]['birth']))
        else:
            full_result.append(result[i]['first'] + " " + result[i]['middle'] + " " + result[i]['last']
                               + ", born " + str(result[i]['birth']))
    for row in full_result:
        print(row)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python roster.py {house_name}")
    else:
        house_name = sys.argv[1]
        main(house_name)
