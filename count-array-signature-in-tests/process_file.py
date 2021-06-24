#!/bin/python3

import re

array_const_re = re.compile('.*Array.*')
select_re = re.compile('.*select.*')
store_re = re.compile('.*store.*')
count_array_consts = 0
count_select_disjoint_previous = 0
count_store_disjoint_previous = 0
total = 0

f = open("file.txt", 'r')
curr_line = ""
while True:
    line = f.readline()
    if not line:
        break
    if(line[0] == ';'):
        if(array_const_re.match(curr_line)):
            count_array_consts += 1
        else:
            if(select_re.match(curr_line)):
                count_select_disjoint_previous += 1
            else:
                if(store_re.match(curr_line)):
                    count_store_disjoint_previous += 1
        total += 1
        curr_line = ""
    else:
        curr_line += line[0:-1]

count = count_array_consts + count_select_disjoint_previous + count_store_disjoint_previous
print(count)
print(total)
