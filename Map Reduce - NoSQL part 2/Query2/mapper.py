import sys

for line in sys.stdin:

    line = line.strip()
    sender_dept , reciever_dept  = line.split(" ",1) # converting into the key value pair

    if(sender_dept != reciever_dept): # ignoring the self loops
        print(sender_dept, reciever_dept)
        