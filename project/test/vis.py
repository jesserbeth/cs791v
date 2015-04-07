from pylab import *
from numpy import outer
import csv

# filename = raw_input("Enter file name: ")
# print filename
A = []
# with open(filename, 'rb') as csvfile:
with open('data_Roger__2048.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=' ')
	for row in spamreader:
		T = []
		for item in row:
			if item != '':
				item = int(item, 10)
				T.append(float(item))
		A.append(T)
		# print ', '.join(row)
	# for item in spamreader:
	# 	# A.append(float(item))
	# 	print item
	# 	if item != '':
	# 		item = float(item)
# asdf
# for item in A:
# 	print item

# A = spamreader
# figure(1)
# imshow(A, interpolation='nearest')
# grid(True)

# figure(2)
# imshow(A, interpolation='bilinear')
# grid(True)

figure(3)
imshow(A, interpolation='none')
imshow(A)
grid(True)
show()
