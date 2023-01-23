import sys

MAX = 10
top_10_sender = [0]*MAX
top_10_mail_count = [0]*MAX

for line in sys.stdin:
    line = line.strip()
    sender_id , mail_count = line.split(" ")

    # print(sender_id,mail_count)
    if int(mail_count) > top_10_mail_count[-1]:
        for index in range(MAX):
            if int(mail_count) > top_10_mail_count[index]:
                top_10_sender.insert(index,sender_id )
                top_10_mail_count.insert(index,int(mail_count) )
                top_10_sender.pop()
                top_10_mail_count.pop()
                break

for index in range(MAX):
    print(top_10_sender[index])




