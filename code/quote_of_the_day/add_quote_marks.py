import csv

with open('quotes.csv', newline='\n') as csv_file:
	csv_reader = csv.reader(csv_file, quotechar=None)
	new_csv_file = open("new_quotes.csv", 'w', newline='\n')
	csv_writer = csv.writer(new_csv_file, quotechar=None)
	for row in csv_reader:
		if len(row) == 3:
			row[1] = "\"" + row[1] + "\""
		csv_writer.writerow(row)

