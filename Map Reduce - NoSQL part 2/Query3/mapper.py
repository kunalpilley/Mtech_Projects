import sys

for line in sys.stdin:

    line = line.strip()
    sender_dept , reciever_dept  = line.split(" ",1)

    if(sender_dept != reciever_dept): # avoiding the self loops 
        print(sender_dept, reciever_dept)