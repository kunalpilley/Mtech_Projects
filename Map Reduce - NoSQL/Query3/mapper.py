file1 = open('../Query2/task2.txt', 'r')
lines = file1.read()
lines.strip()
top_employees_list = lines.split('\n')
top_employees_list.pop()  # del edge_list[-1]


file2 = open("network.txt","r")
lines = file2.read()
lines.strip()
edge_list = lines.split('\n')
edge_list.pop() 

lenght = len(edge_list)

for i in range (lenght):
    edge_list[i] = edge_list[i].strip()
    sender , reciever = edge_list[i].split(" ")
    if reciever in top_employees_list:
        if sender != reciever:
            # print(sender, reciever)
            print(reciever, sender)





file1.close()
file2.close()