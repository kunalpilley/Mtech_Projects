import sys

for line in sys.stdin:

    line = line.strip()
    sender_dept , reciever_dept  = line.split(" ",1) # converting into the key value pair
    print(sender_dept, reciever_dept)