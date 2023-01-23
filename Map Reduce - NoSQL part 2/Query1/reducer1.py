import sys

dept_pair_dict = {}

for line in sys.stdin:
    line = line.strip()
    sender_dept , reciever_dept  = line.split(" ",1)
    # for all file recived from corresponding combiner 
    # taking the correspondance only once and making unique instance
    if dept_pair_dict.get(sender_dept+" "+reciever_dept) == None:
            dept_pair_dict[sender_dept+" "+reciever_dept] = 1
            dept_pair_dict[reciever_dept+" "+sender_dept] = 1
for key,val in dept_pair_dict.items():
    print(key)
