import sys

current_id = None
previous_id = ""
counter = 1
final_dept = 0 
final_count = 0
for line in sys.stdin:

    line = line.strip()
    dept_id , _ = line.split(" ",1)

    if current_id == None:
        current_id = dept_id
    elif current_id == dept_id:
        counter = counter +1
    elif current_id != dept_id:
        previous_id = current_id
        current_id = dept_id
        #print(previous_id, counter)

        if(counter > final_count):
            final_count = counter
            final_dept = previous_id

        counter = 1

#print(dept_id, counter)
if(counter > final_count):
     final_count = counter
     final_dept = dept_id
     
#print(" final totals\n")
print(final_dept, final_count)

