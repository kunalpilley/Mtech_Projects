
file = open("network.txt","r")
lines = file.read()
lines.strip()
file.close()

edge_list = lines.split('\n')
edge_list.pop()  # del edge_list[-1]

lenght = len(edge_list)

for i in range (lenght):
    edge_list[i] = edge_list[i].strip()

for i in range (lenght):
    sender , reciever = edge_list[i].split(" ")

    if sender != reciever:
        print(edge_list[i])
