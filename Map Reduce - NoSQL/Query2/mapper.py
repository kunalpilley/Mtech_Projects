file = open('../Query1/task1.txt', 'r')
lines = file.read()
lines.strip()
file.close()

mail_count_list = lines.split('\n')
mail_count_list.pop()  # del edge_list[-1]

lenght = len(mail_count_list)

for i in range (lenght):
    mail_count_list[i] = mail_count_list[i].strip()

for i in range (lenght):
    sender , reciever = mail_count_list[i].split(" ")
    print(sender,reciever)