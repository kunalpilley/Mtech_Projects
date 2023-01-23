from os import PRIO_PGRP
import requests
import re
import ply.lex as lex
import ply.yacc as yacc
from wordcloud import WordCloud, STOPWORDS
import matplotlib.pyplot as plt
import pandas as pd
import news
import response
from datetime import datetime
from time import sleep
import sys


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


def convert_date(date1,date2) :

    split_date1 = date1.split('-',2)
    split_date2 = date2.split('-',2)

    split_date1[0] = int(split_date1[0])
    split_date2[0] = int(split_date2[0])

    split_date1[1] = chnage_month(split_date1[1])
    split_date2[1] = chnage_month(split_date2[1])

    date1 = str(split_date1[0]) + " " + split_date1[1] + " " + split_date1[2]
    date2 = str(split_date2[0]) + " " + split_date2[1] + " " + split_date2[2]

    print(" date 1")
    print(date1)
    print(" date 2")
    print(date2)

    return date1, date2

def convert_date_obj(date1,date2) :

    date_object1 = datetime.strptime(date1, "%d-%m-%Y")
    print("date_object =", date_object1)  

    date_object2 = datetime.strptime(date2, "%d-%m-%Y")
    print("date_object =", date_object2) 

    return   date_object1,date_object2


def fetch_news(date1, date2):
    print("\n****************************************************************************************************")
    print("\n                                         Fetching News                                            \n")
    print("****************************************************************************************************\n")
    print("element in dates:",len(news.all_dates))
    print("element in news:",len(news.all_news))

    for i in range (len(news.all_dates)):
        if(date1==news.all_dates[i]):
            print("found match 1")
            break

    for j in range (len(news.all_dates)):
        if(date2==news.all_dates[j]):
            print("found match 2")
            break

    for k in range (len(news.all_dates)):
        if k >=i and k<=j:
            
            print("\n",news.all_dates[k])
            print("\n",news.all_news[k],"\n\n")
            #world_could_str =world_cloud_str+" "+ all_news[k]

    temp_list = news.all_news[i:j+1]
    world_cloud_str = ' '.join(map(str, temp_list))
    return world_cloud_str


def fetch_response(date1,date2):
    print("\n****************************************************************************************************")
    print("\n                                         Fetching Response                                        \n")
    print("****************************************************************************************************\n")
    print("element in dates:",len(response.all_dates_final))
    print("element in news:",len(response.all_news_final))

    for i in range (len(response.all_dates_final)):
        if(date1 == response.all_dates_final[i]):
            print("found match 1")
            break

    for j in range (len(response.all_dates_final)):
        if(date2 == response.all_dates_final[len(response.all_dates_final)-1-j]):
            print("found match 2")
            break

    j = len(response.all_dates_final) -1 - j

    world_cloud_str = ''
    for k in range (len(response.all_dates_final)):
        if k >=i and k<=j:
            print("\n",response.all_dates_final[k],"\n")
            print("\n",response.all_news_final[k],"\n\n")

    temp_list = response.all_news_final[i:j+1]
    world_cloud_str = ' '.join(map(str, temp_list))
    return world_cloud_str

def fetch_response_object(date1,date2):
    print("\n****************************************************************************************************")
    print("\n                                         Fetching Response                                        \n")
    print("****************************************************************************************************\n")
    print("element in dates:",len(response.all_dates_final_objects))
    print("element in news:",len(response.all_news_final))

    temp_list = []
    for i in range (len(response.all_dates_final_objects)):
        if response.all_dates_final_objects[i]>=date1 and response.all_dates_final_objects[i]<=date2 :
            print("\n",response.all_dates_final[i],"\n")
            print("\n",response.all_news_final[i],"\n\n")
            data = response.all_news_final[i]
            temp_list.append(data)

    world_cloud_str = ''

    world_cloud_str = ' '.join(map(str, temp_list))
    if len(world_cloud_str)== 0:
        world_cloud_str = "No Response Found in the Given Range"
    return world_cloud_str

def fetch_news_object(date1,date2):
    print("\n****************************************************************************************************")
    print("\n                                         Fetching News                                        \n")
    print("****************************************************************************************************\n")
    print("element in dates:",len(news.all_dates_objects))
    print("element in news:",len(news.all_news))

    temp_list = []
    for i in range (len(news.all_dates_objects)):
        if news.all_dates_objects[i]>=date1 and news.all_dates_objects[i]<=date2 :
            print("\n",news.all_dates[i],"\n")
            print("\n",news.all_news[i],"\n\n")
            data = news.all_news[i]
            temp_list.append(data)

    world_cloud_str = ''

    world_cloud_str = ' '.join(map(str, temp_list))

    if len(world_cloud_str)== 0:
        world_cloud_str = "No News Found in the Given Range"

    return world_cloud_str

def get_country_range(country_name) :
    sleep(3)
    countries = {'Argentina': 'February,2020 - January,2021', 'Australia': 'January,2020 - February,2022',
    'Bangladesh': 'No Date Range Found', 'Brazil': 'November,2019 - January,2022', 'Ontario': 'January,2020 - March,2022', 
    'Ghana': 'March,2020 - February,2022', 'India': 'January,2020 - May,2021', 'Indonesia': 'Feburary,2020 - December,2021', 
    'Republic of Ireland': 'January,2020 - March,2022', 'Malaysia': 'January,2020 - March,2022', 'Mexico': 'January,2020 - Augest,2020'
    , 'New Zealand': 'February,2020 - March,2022', 'Nigeria':  'February,2020 - December,2020',
    'Pakistan':  'January,2020 - March,2021', 'Philippines': 'January,2020 - March,2022', 
    'Russia': 'January,2020 - December,2020 ', 'Singapore': 'January,2020 - March,2022', 
    'South Africa': 'March,2020 - January,2022', 'Spain': 'January,2020 - March,2022', 
    'Turkey': 'January,2020 - January,2021', 'United Kingdom': 'January,2020 - December,2020',
    'United States': 'January,2020 - December,2021'}

    if country_name in countries:
        print(f"\nThe Date range fo the news for {country_name} is as follows:\n")
        print(countries[country_name])
    else:
        print("\ncountry name does not exist/ Wrong name enteretred\n")

def generate_WC(world_cloud_str):

    print("\n\ngenerating Word Cloud\n\n")
    print("\n\nClose Word Cloud to Continue\n\n")
    stopwords = set(STOPWORDS)

    wordcloud = WordCloud(width = 800, height = 800,
                background_color ='red',
                stopwords = stopwords,
                min_font_size = 10).generate(world_cloud_str)
 
    # plot the WordCloud image                      
    plt.figure(figsize = (8, 8), facecolor = None)
    plt.imshow(wordcloud)
    plt.axis("off")
    plt.tight_layout(pad = 0)
    plt.show()  
    return wordcloud

def comman_word(news1, news2):
    print("\nVarifying Comman Word \n")

    # news1 = [x.lower() for x in news1]
    # news2 = [x.lower() for x in news2]

    # str1 = ' '.join(map(str, news1))
    # str2 = ' '.join(map(str, news2))

    # print("first string whole ",str1)
    # print("first string whole ",str2)
    str1 = news1.lower()
    str2 = news2.lower()
    
    list1 = str1.split()
    list2 = str2.split()
    # print("list word wise  ",list1)
    # print("2 list word wise ",list2)  
    #      
    temp1 = set(list2)
    list3 = [value for value in list1 if value in temp1]
    temp2 = set(list1)
    list4 = [value for value in list2 if value in temp2]
    comman_list = list3+list4
    world_cloud_str = ' '.join(map(str, comman_list))
    # print("FINAL STR: ",world_cloud_str)
    print("\nGeneration Word Cloud for Comman word in both the date range\n")
    word_cloud_comman_word = generate_WC(world_cloud_str)
    return word_cloud_comman_word


def covid_comman_word(news1, news2):
    print("\nVarifying Covid-Comman Word \n")

    FileName = 'covid_word_dictionary.txt'
    covid_words = open(FileName, 'r', errors = 'ignore').read()
    covid_words = covid_words.lower()
    raw = covid_words.split()

    news1  = news1+news2
    # news1 = [x.lower() for x in news1]

    # str1 = ' '.join(map(str, news1))
    
   
    list1 = news1.split()
    # print("list word wise for whole data  ",list1)

    temp1 = set(raw)
    list3 = [value for value in list1 if value in temp1]
    world_cloud_str = ' '.join(map(str, list3))

    # print("FINAL STR:\n ",world_cloud_str)
    
    print("\nGeneration Word Cloud for Covid - Comman word in both the date range\n")

    WC_covid_comman_word = generate_WC(world_cloud_str)

    return WC_covid_comman_word


def top_twenty_word(WC_comman_word):

    print("\n\nTop 20 common words:\n")
    i = 1
    for each in WC_comman_word.words_.keys():
        if i<=20:
            print(i,"-->",[each])
            i +=1
        else:
            break
    

while(1):
    print('''
**************************** Main Menu **************************

1. News and Response in a given date range  with plotting (Task 1) 
2. News in two diffrent time range with ploatting (Task 2)
3. Date range for a Country's information
4. News for a country in given duraion and plotting
5. Jaccard Similarity for given country 
6. To enter modules from Assigment 4
7. Exit

******************************************************************

"Enter Your Choice :"

''')
    choice = int(input())

    if choice == 1:
        print("inside 1")
        date1 = input("\nenter starting date :\n")            # ask for two dates
        date2 = input("\nenter End date :\n")
        
        date1_news,date2_news = convert_date_obj(date1,date2)             # convert then to req format

        word_cloud_string1 = fetch_news_object(date1_news, date2_news)        # access news data 
        generate_WC(word_cloud_string1)  
        
        date1_res,date2_res = convert_date_obj(date1,date2)                    #  print WC
    
        word_cloud_string2 = fetch_response_object(date1_res, date2_res)    # access response  data 
        generate_WC(word_cloud_string2)                      #  print WC
        

    elif choice == 2:
        print("inside 2\n")
        date1 = input("\nenter starting date - 1:\n")       # ask for  dates -1st range   
        date2 = input("\nenter End date - 1:\n")
        date1,date2 = convert_date_obj(date1,date2)         # convert then to req format
        
        date3 = input("\nenter starting date - 2:\n")       # ask for  dates -2nd range
        date4 = input("\nenter End date  -2:\n")
        date3,date4 = convert_date_obj(date3,date4)         # convert then to req format
    
        news1 = fetch_news_object(date1, date2)                # access news data 1st range        
        news2 = fetch_news_object(date3, date4)                # access news data 2nd range
        
        WC_comman_word =comman_word(news1,news2)        # workout comman word and print word cloud

        WC_covid_comman_word = covid_comman_word(news1,news2)   # workout comman word and print word cloud

        print("\nPercentage of covid related words in common words of news :\n")
        print((len(WC_covid_comman_word.words_.keys())/len(WC_comman_word.words_.keys()))*100)

      
        print("\nTop Twenty Comman Words from News \n")
        top_twenty_word(WC_comman_word)

        print("\nTop Twenty Covid Related Comman Words from News \n")
        top_twenty_word(WC_covid_comman_word)


    elif choice == 3:
        
        print("\nEnter Counry name with first letter capilalised\n")
        country_name = input("A wrong formate or Invalid name will not fetch the result:\n")

        get_country_range(country_name)

    elif choice == 4:
        print("\nCountry names are : Australia,Ghana,India,Indonesia,Republic of Ireland,\nMalaysia,New Zealand,Philippines,Russia,Singapore\n,United Kingdom,United States\n\n")
        name = input('\nEnter Country Name : ')
        date1 = input("\nenter starting date :\n")            # ask for two dates
        date2 = input("\nenter End date :\n")
        import country_news
        country_news.fetch_news(name,date1,date2)
        del sys.modules['country_news']

    elif choice == 5:
        print("\n***** functionality not implemented *******\n")
        

    elif choice == 6:
        print("\n executing Assignmnet 4 tasks\n")
        import assng4
        del sys.modules['assng4']
        print("\n\back to  Assignmnet 5 tasks\n\n")
        
    elif choice == 7:
        print("terminating operations")
        break
    else :
        print("invalid input!! Please try again")

