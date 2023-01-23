from os import PRIO_PGRP
import requests
import re
import ply.lex as lex
import ply.yacc as yacc
from wordcloud import WordCloud, STOPWORDS
import matplotlib.pyplot as plt
import pandas as pd
from datetime import datetime

headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36'}
print("\n\nImporting World Response from january 2020 onwards\n")
print("This will take about 30 seconds\n\n")
dd = []
mm = []
yyyy = []
NEWS = []
all_dates = []
all_news = []

DD_final = []
MM_final = []
YYYY_final = []
NEWS_final = []
all_dates_final = []
all_news_final = []

def sort():
    total_dates = len(dd)

    # on years 
    # for i in range(total_dates-1):
    #     for j in range(i+1, total_dates-1):
    #         if int(mm[j]) < int(mm[i]) :

    #             yyyy[j], yyyy[i] = yyyy[i], yyyy[j]
    #             mm[j], mm[i] = mm[i], mm[j]
    #             dd[j], dd[i] = dd[i], dd[j]
    #             all_news[j], all_news[i] = all_news[i], all_news[j]
    #             all_dates[j], all_dates[i] = all_dates[i], all_dates[j]

    # on years 
    # for i in range(total_dates-1):
    #     for j in range(0, total_dates-i-1):
    #         if int(YYYY[j]) > int(YYYY[j + 1]) :

    #             YYYY[j], YYYY[j + 1] = YYYY[j + 1], YYYY[j]
    #             MM[j], MM[j + 1] = MM[j + 1], MM[j]
    #             DD[j], DD[j + 1] = DD[j + 1], DD[j]
    #             all_news[j], all_news[j + 1] = all_news[j + 1], all_news[j]
    #             all_dates[j], all_dates[j + 1] = all_dates[j + 1], all_dates[j]

    
    # # on months
    # for i in range(total_dates-1):
    #     for j in range(0, total_dates-i-1):
    #         if int(MM[j]) > int(MM[j + 1]) :

    #             YYYY[j], YYYY[j + 1] = YYYY[j + 1], YYYY[j]
    #             MM[j], MM[j + 1] = MM[j + 1], MM[j]
    #             DD[j], DD[j + 1] = DD[j + 1], DD[j]
    #             all_news[j], all_news[j + 1] = all_news[j + 1], all_news[j]
    #             all_dates[j], all_dates[j + 1] = all_dates[j + 1], all_dates[j]

    # on date
    for i in range(total_dates-1):
        for j in range(0, total_dates-i-1):
            if int(dd[j]) > int(dd[j + 1]) :

                yyyy[j], yyyy[j + 1] = yyyy[j + 1], yyyy[j]
                mm[j], mm[j + 1] = mm[j + 1], mm[j]
                dd[j], dd[j + 1] = dd[j + 1], dd[j]
                all_news[j], all_news[j + 1] = all_news[j + 1], all_news[j]
                all_dates[j], all_dates[j + 1] = all_dates[j + 1], all_dates[j]

def month_formate(month_str):

    if month_str == 'January':
        return '01'

    if month_str == 'February':
        return '02'

    if month_str == 'March':
        return '03'

    if month_str == 'April':
       return '04'

    if month_str == 'May':
        return '05'

    if month_str == 'June':
        return '06'

    if month_str == 'July':
        return '07'

    if month_str == 'August':
        return '08'

    if month_str == 'September':
        return '09'

    if month_str == 'October':
        return '10'

    if month_str == 'November':
        return '11'

    if month_str == 'December':
        return '12'

def chnage_month(month):

    if month == '01':
        return'January'

    if month == '02':
       return'February'

    if month == '03':
        return'March'

    if month == "04":
        return 'April'

    if month == '05':
        return'May'

    if month == '06':
       return 'June'

    if month == '07':
        return 'July'

    if month == '08':
        return 'August'

    if month == '09':
        return 'September'

    if month == '10':
        return 'October'

    if month == '11':
        return'November'

    if month == '12':
        return 'December'



main_url='https://en.wikipedia.org/wiki/Responses_to_the_COVID-19_pandemic_in_'

months = ['January_2020', 'February_2020', 'March_2020', 'April_2020',
                'May_2020', 'June_2020', 'July_2020', 'August_2020', 'September_2020',
                'October_2020', 'November_2020', 'December_2020', 'January_2021', 'February_2021',
                'March_2021', 'April_2021', 'May_2021', 'June_2021', 'July_2021',
                'August_2021', 'September_2021', 'October_2021', 'November_2021', 'December_2021',
                'January_2022', 'February_2022','March_2022']

# months = ['January_2020', 'February_2020']

total_months = len(months)

tokens = ['DATE_ANCHOR', 'DATE', 'NEWS_LIST','STOP_POINT']

def t_STOP_POINT(t):
    r'<h3>'
    return t

def t_DATE_ANCHOR(t):
    r'<span.class=\"mw-headline\".id=\"\d'
    return t

def t_DATE(t):
    r'>\d{1,2}\s\w{3,9}<'
    return t

def t_NEWS_LIST(t):
    r'((<li>.+</li>\s*)+|(<p>.+\s</p>\n<ul>)+|(<p>.*\n</p>)+)+'
    return t


def t_error (t):
    t.lexer.skip(1)
    return


def p_start(t):
    '''start : news'''

def p_news(t):
    '''news : DATE_ANCHOR DATE NEWS_LIST'''

    str_date = t[2]
    str_date  = str(str_date[1:-1])

    temp1 = str_date.split()
    Date = temp1[0]

    temp = months[i].split('_',1)
    Month = month_formate(temp[0])
    Year = temp[1]
    str_date = str_date+" "+temp[1]

    str_news = t[3]
    str_news = re.sub("<.*?>", "", str_news)

    dd.append(Date)
    mm.append(Month)
    yyyy.append(Year)
    all_dates.append(str_date)
    all_news.append(str_news)

    # print(str_date)
    # print(str_news)


def p_error(p):
    pass

for i in range(total_months):
   
    month_url=main_url+months[i]
    webpage = requests.get(url = month_url,headers=headers)

    #print('Web page saved for : ', i+1 ,' ',months[i] )
    page_content = webpage.text
    page_content1 = re.sub("<meta.*?>","",page_content)
    page_content2 = re.sub("<sup.*?</sup>","",page_content1)
    page_content3 = re.sub("<a\shref.*?\">|</a>","",page_content2)
    page_content4 = re.sub("<li\sid.*\n</li>","",page_content3)
    page_content5 = re.sub("<ul><li><b>COVID-19</b>\s\(disease\)</li>(\n.*)+</li>","",page_content4) 

    # filename = months[i]+".html"

    # with open(filename,'w',encoding='utf8') as file_pointer:
    #     file_pointer.write(page_content5)
    # file_pointer.close


    lexer = lex.lex()
    lexer.input(page_content5)

    parser = yacc.yacc()
    parser.parse(page_content5)

    sort()

    DD_final = DD_final + dd
    MM_final = MM_final + mm
    YYYY_final = YYYY_final + yyyy
    all_dates_final = all_dates_final + all_dates
    all_news_final = all_news_final + all_news

    dd.clear()
    mm.clear()
    yyyy.clear()
    all_dates.clear()
    all_news.clear()




print("element in dates:",len(all_dates_final))
print("element in news:",len(all_news_final))

all_dates_final_objects = []
for i in range(len(all_dates_final)):
    all_dates_final_objects.append(datetime.strptime(all_dates_final[i], "%d %B %Y"))
    # print("date_object =", all_dates_final_objects[i])

print("element in dates object:",len(all_dates_final_objects))

# for i in range(len(all_dates_final_objects)):
#     print("date_object =", all_dates_final_objects[i])



