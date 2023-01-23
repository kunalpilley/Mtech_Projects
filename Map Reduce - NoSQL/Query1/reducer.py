import sys

current_id = None
previous_id = ""
counter = 1
for line in sys.stdin:

    line = line.strip()
    sender_id , _ = line.split(" ",1)

    if current_id == None:
        current_id = sender_id
    elif current_id == sender_id:
        counter = counter +1
    elif current_id != sender_id:
        previous_id = current_id
        current_id = sender_id
        print(previous_id, counter)
        counter = 1
print(sender_id, counter)
