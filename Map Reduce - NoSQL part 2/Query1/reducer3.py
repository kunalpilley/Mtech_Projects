import sys

dept_pair_dict = {}

for line in sys.stdin:
    line = line.strip()
    _ , receiver_dept  = line.split(" ",1)

     # reducing the entire data as per each sender receiver with same no of communication with other dept 
    if dept_pair_dict.get(receiver_dept) == None:
            dept_pair_dict[receiver_dept] = 1
    else:
        dept_pair_dict[receiver_dept] = dept_pair_dict[receiver_dept] + 1
for key,val in dept_pair_dict.items():
    print(key,val)
