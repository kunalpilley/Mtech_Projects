import sys

dept_pair_dict = {}
l = []
sum = 0

for line in sys.stdin:
    line = line.strip()
    l.append(line)
    
    # counting no of time a node pair have conversation
    if line not in dept_pair_dict:
            dept_pair_dict[line] = 1
    else:
        dept_pair_dict[line] = dept_pair_dict[line] + 1


# printing node pair having conversation more than 50 times as strongly connected 

for key,val in dept_pair_dict.items():
    if val >= 50:
        print(key)
        sum = sum + val  
      

