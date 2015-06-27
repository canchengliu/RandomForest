import sys
"""
	convert
	id, value0, value1, value2, value3, value4, value5 ... label
	to
	label index1:value1 index2:value2 ...
"""
sourcefile = open('data/train100.csv', 'r')
svmfile = open('data/', 'w')
items = []
maxval = []
minval = []
first_line = ''
for line in sourcefile:
	if first_line == '':
		first_line = line
		continue
	items = line.split(', ')
	for item in items[1:-1]:
		maxval = 
	#item_id = items[0]
	item_label = items[-1]
	new_line = item_label[:-1] + ' '
	i = 1
	for item in items[1:-1]:
		#new_line += str(i) + ':' + str(int(float(item) * 100)) + ' '
		new_line += str(i) + ':' + item + ' '
		i += 1
	svmfile.write(new_line + '\n')

sourcefile.close()
svmfile.close()

bin = 3
a = [1, 2, 3, 4, 5, 6, 7, 8]
b = []
lena = len(a)
perlen = lena / bin
rest = lena - perlen * bin 
pre = 0
for i in range(0, bin):
	if i < rest:
		b.append(a[pre:pre+perlen + 1])
		pre += perlen + 1
	else:
		b.append(a[pre:pre+perlen])
		print pre
		pre += perlen