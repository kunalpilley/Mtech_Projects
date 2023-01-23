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
str_news =""
all_dates = []
all_news = []

print("\n\nImporting World News from january 2020 onwards\n")
print("This will take about 30 seconds\n")

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


main_url='https://en.wikipedia.org/wiki/Timeline_of_the_COVID-19_pandemic_in_'
months = ['January_2020', 'February_2020', 'March_2020', 'April_2020',
                'May_2020', 'June_2020', 'July_2020', 'August_2020', 'September_2020',
                'October_2020', 'November_2020', 'December_2020', 'January_2021', 'February_2021',
                'March_2021', 'April_2021', 'May_2021', 'June_2021', 'July_2021',
                'August_2021', 'September_2021', 'October_2021', 'November_2021', 'December_2021',
                'January_2022', 'February_2022','March_2022']

# months = ['February_2020']
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
    str_date  = str_date[1:-1]
    temp = months[i].split('_',1)
    str_date = str_date+" "+temp[1]
    str_news = t[3]
    str_news = re.sub("<.*?>", "", str_news)

    all_dates.append(str_date)
    all_news.append(str_news)
  

def p_error(p):
    pass

for i in range(total_months):
   
    month_url=main_url+months[i]
    webpage = requests.get(url = month_url,headers=headers)

    # print('Web page saved for : ', i+1 ,' ',months[i] )
    page_content = webpage.text
    page_content1 = re.sub("<meta.*?>","",page_content)
    page_content2 = re.sub("<sup.*?</sup>","",page_content1)
    page_content3 = re.sub("<a\shref.*?\">|</a>","",page_content2)
    page_content4 = re.sub("<li\sid.*\n</li>","",page_content3)
    page_content5 = re.sub("<ul><li><b>COVID-19</b>\s\(disease\)</li>(\n.*)+</li>","",page_content4) 

 

 

    lexer = lex.lex()
    lexer.input(page_content5)

    parser = yacc.yacc()
    parser.parse(page_content5)

    
print("element in dates:",len(all_dates))
print("element in news:",len(all_news))

all_dates_objects = []
for i in range(len(all_dates)):
    all_dates_objects.append(datetime.strptime(all_dates[i], "%d %B %Y"))
    # print("date_object =", all_dates_final_objects[i])

print("element in dates object:",len(all_dates_objects))

# for i in range(len(all_dates_objects)):
#     print("date_object =", all_dates_objects[i])

  
        