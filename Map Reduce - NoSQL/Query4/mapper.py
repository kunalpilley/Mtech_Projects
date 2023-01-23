
department_list = []

file1 = open('dept_labels.txt', 'r')
employee_department= file1.read()
employee_department.strip()
employee_department_list = employee_department.split('\n')
employee_department_list.pop()  # del edge_list[-1]

file2 = open("network.txt","r")
lines = file2.read()
lines.strip()
edge_list = lines.split('\n')
edge_list.pop() 


for i in range (len(employee_department_list)):
    employee, department = employee_department_list[i].split()
    department_list.append(department)

for i in range (len(edge_list)):
    sender , reciever = edge_list[i].split(" ")
    sender = int(sender)
    reciever = int(reciever)

    if sender != reciever and department_list[sender] != department_list[reciever]:

        print(department_list[sender],department_list[reciever])

file1.close()
file2.close()