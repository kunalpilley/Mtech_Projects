import sys

dept_pair_dict = {}

for line in sys.stdin:
    line = line.strip()
    sender_dept , _  = line.split(" ",1)
    # reducing the entire data as per each sender reciver pair 
    if dept_pair_dict.get(sender_dept) == None:
            dept_pair_dict[sender_dept] = 1
    else:
        dept_pair_dict[sender_dept] = dept_pair_dict[sender_dept] + 1
for key,val in dept_pair_dict.items():
    print(key,val)
