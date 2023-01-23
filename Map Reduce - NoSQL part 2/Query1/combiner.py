import sys

unique_sender_receiver_pair = []

for line in sys.stdin:
    line = line.strip()
    sender_dept , reciever_dept  = line.split(" ",1)

    # convertint the edge relationship in to symmetric values
    # for each file taking the correspondance only once to reduce the amount of data being sent to the reducer
    if line not in unique_sender_receiver_pair: 
        unique_sender_receiver_pair.append(sender_dept+" "+reciever_dept)
        unique_sender_receiver_pair.append(reciever_dept+" "+sender_dept)

for i in unique_sender_receiver_pair:
    print(i)

    