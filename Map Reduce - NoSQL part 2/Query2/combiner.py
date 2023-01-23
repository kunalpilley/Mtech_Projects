import sys

unique_sender_receiver_pair = []

for line in sys.stdin:
  
    line = line.strip("\n\r")
    sender_dept , reciever_dept  = line.split(" ")
    sender_dept = int(sender_dept.strip())
    reciever_dept = int(reciever_dept.strip())

    # convertint the edge relationship in to symmetric values
    # for each file taking the correspondance of pair as symmtric value to 
    # reduce the amount of data being sent to the reducer
    if sender_dept > reciever_dept:
        dept_pair = str(reciever_dept) + " " + str (sender_dept)
    else:
        dept_pair = str(sender_dept) + " " + str (reciever_dept)

    unique_sender_receiver_pair.append(dept_pair)
        
for i in unique_sender_receiver_pair:
    print(i)


    