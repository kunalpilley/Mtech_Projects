from time import sleep
import ply.lex as lex
import ply.yacc as yacc
import re
from datetime import datetime
import requests
from wordcloud import WordCloud, STOPWORDS
import matplotlib.pyplot as plt



url = 'https://en.wikipedia.org/wiki/Timeline_of_the_COVID-19_pandemic'
response = requests.get(url)
file = open('sourcePage.html', 'w')
file.write(response.text)
file.close()


months = {'January': 1, 'February': 1, 'March': 1, 'April': 1, 'May': 1, 'June': 1,
          'July': 1, 'August': 1, 'September': 1, 'October': 1, 'November': 1, 'December': 1}

countries = {'Argentina': 3 , 'Australia': 1 ,  'Bangladesh': 3,  'Brazil': 3 ,
'Ontario': 2 ,  'Ghana': 2 ,  'India': 1,  'Indonesia': 1 ,  'Republic of Ireland': 2,  
'Malaysia': 2,  'Mexico': 3, 'New Zealand': 2,  'Nigeria': 3,  
'Pakistan': 3,  'Philippines': 2,  'Russia': 2,  'Singapore': 2 ,  'South Africa': 3, 
 'Spain': 3,  'Turkey': 3,  'United Kingdom': 2,  'United States': 2 }

global news_list
news_list = {}
country_data_all = {}

# ----------------------Functions-------------------
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
# --------------------------------------------------
def convert_date(date1,date2) :

    split_date1 = date1.split('-',2)
    split_date2 = date2.split('-',2)


    split_date1[1] = chnage_month(split_date1[1])
    split_date2[1] = chnage_month(split_date2[1])

    date1 = split_date1[1] + " " + split_date1[2]
    date2 = split_date2[1] + " " + split_date2[2]

    print(" date 1")
    print(date1)
    print(" date 2")
    print(date2)

    return date1, date2
#---------------------------------------------------
def get_url(Country_name):
    text = open('sourcePage' + '.html', 'r').read()
    data = re.findall(
        '"Timeline_by_country">Timeline by country(?s:.*?)Worldwide_cases_by_month_and_year', str(text))
    link = re.findall('<a href="(.*)" title.*>' +
                      Country_name+' .*</a>', str(data[0]))
    links = []
    for i in link:
        i = 'https://en.wikipedia.org' + i.split('&')[0]
        links.append(i)
    return links
# ----------------------------------------------


def print_news(country_name, start_date, end_date):
    word_cloud_str = ''
    date1 =  datetime.strptime(start_date, '%B %Y')
    date2 = datetime.strptime(end_date, '%B %Y')
    if country_data_all.get(country_name) == None:
        print('invalid country')
    else:
        date_keys = country_data_all[country_name].keys()
        for i in date_keys:
            dates = datetime.strptime(i, '%B %Y')
            if(dates >= date1 and dates <= date2):
                print("\n")
                print(i)
                print("\n")
                print(country_data_all[country_name][i])
                print("\n")
                word_cloud_str += ' ' + country_data_all[country_name][i]

       
        if(len(word_cloud_str)==0):
            word_cloud_str = "\n no data found in range try agsain\n"

        print("\n\ngenerating Word Cloud\n\n")
        print("\n\nClose Word Cloud to Continue\n\n")
        stopwords = set(STOPWORDS)

        wordcloud = WordCloud(width = 800, height = 800,
                    background_color ='red',
                    stopwords = stopwords,
                    min_font_size = 10).generate(word_cloud_str)
    
        # plot the WordCloud image                      
        plt.figure(figsize = (8, 8), facecolor = None)
        plt.imshow(wordcloud)
        plt.axis("off")
        plt.tight_layout(pad = 0)
        plt.show()  
        print('Word cloud generated')
        return wordcloud
        
# ----------------------------------------------

def fetch_news(country_name, date1, date2):
    if countries[country_name] == 3:
        print('\nNo Record Found for the country ! try again ')
        print('\n')
    else:

        if country_data_all.get(country_name) != None:
            return
        if countries.get(country_name) == None:
            return
        else:
            links = get_url(country_name)
            for i in links:
                res1 = requests.get(i)
                if res1.status_code == 200:
                    year = re.findall('\d+', str(i))[-1]
                    if countries[country_name] == 1:
                        list1_countries(res1, year)
                    if countries[country_name] == 2:
                        list2_countries(res1, year)
            temp_list = news_list.copy()
            country_data_all[country_name] = temp_list
            news_list.clear()

        date1, date2 = convert_date(date1,date2)

        print_news(country_name, date1, date2)


# ---------------------------------------------
tokens = ['DATE', 'NEWS']


def t_DATE(t):
    r'<span.*?>.*?<\/span><\/h2>|<span.*?>.*?<\/span><\/h3>'
    return t


def t_NEWS(t):
    r'((<li>.+</li>\s*)+|(<p>.+\s</p>\n<ul>)+|(<p>.*\n</p>)+)+'
    return t


t_ignore = ' \t\n'


def t_error(t):
    t.lexer.skip(1)
    return

# ----------------------------------------------


def list1_countries(response, year):
    data = re.findall(
        '<h2><span.*>.*</h2>(?s:.*?)id="References">References', response.text)
    if len(data) != 0:
        news = re.split('<h2>', data[0])
        for i in news:
            if len(i) > 0:
                page_content = re.sub("<a\shref.*?\">|</a>", "", i)
                page_content = re.sub("<li\sid.*\n</li> | <ul><li><b>COVID-19</b>\s\(disease\)</li>(\n.*)+</li>", "", page_content)
                page_content = re.sub("<sup .*<\/sup>", "", page_content)
                page_content = re.sub("<td>((.|\n)*)<\/td>", "", page_content)
                page_content = re.sub("<li.*><span.*<\/span><\/li>", "", page_content)
                page_content = re.sub('<span\sclass="mw-editsection">.*?<\/span><\/span>', "", page_content)
                page_content = re.sub('<ul>|</ul>|<i>|</i>|<b>|</b>|<h3>|</h3>|<span\sclass="mw-headlines">.*?<\/span>', "", page_content)
              
                lexer = lex.lex()
                lexer.input(page_content)
                month_wise = ''
                overall_news = ''
                for tok in lexer:
                    if tok.type == 'DATE':
                        date = re.sub('<span.*?>', "", tok.value)
                        date = re.sub('</span>', "", date)
                        date = re.sub('</h2>', "", date)
                        date = date.split(' ')
                        if months.get(date[0]) == None:
                            break
                        else:
                            month_wise = date[0] + ' ' + year
                    if tok.type == "NEWS":
                        news = re.sub('<p>', "", tok.value)
                        news = re.sub('<li>|</li>|<ul>|</ul>|<i>|</i>|</li>|</p>|<div.*?>|<br />', "", news)
                        news = re.sub('</li>', "", news)
                        overall_news += news
                if len(month_wise) > 0:
                    news_list[month_wise] = overall_news

# ----------------------------------------------------


def list2_countries(response, year):
    data = re.findall(
        '<h3><span.*>.*</h3>(?s:.*?)id="References">References|<h3><span.*>.*</h3>(?s:.*?)Notes and references', response.text)
    if len(data) != 0:
        news = re.split('<h3>', data[0])
        for i in news:
            if len(i) > 0:
                page_content = re.sub("<a\shref.*?\">|</a>", "", i)
                page_content = re.sub("<li\sid.*\n</li>|<p><b>WHO<\/b>.*\n<\/p>|<ul><li><b>COVID-19</b>\s\(disease\)</li>(\n.*)+</li>", "", page_content)
                page_content = re.sub("<sup .*<\/sup>", "", page_content)
                page_content = re.sub("<td>((.|\n)*)<\/td>", "", page_content)
                page_content = re.sub("<li.*><span.*<\/span><\/li>", "", page_content)
                page_content = re.sub('<span\sclass="mw-editsection">.*?<\/span><\/span>', "", page_content)
                page_content = re.sub('<ul>|</ul>|<i>|</i>|<b>|</b>', "", page_content)
                page_content = re.sub('<span\sclass="nowrap">.*?<\/span>', "", page_content)

                lexer = lex.lex()
                lexer.input(page_content)
                month_wise = ''
                overall_news = ''
                for tok in lexer:
                    if tok.type == 'DATE':
                        date = re.sub('<span.*?>', "", tok.value)
                        date = re.sub('</span>', "", date)
                        date = re.sub('</h3>', "", date)
                        date = date.split(' ')
                        if months.get(date[0]) == None:
                            break
                        else:
                            month_wise = date[0] + ' ' + year
                    if tok.type == "NEWS":
                        news = re.sub('<p>', "", tok.value)
                        news = re.sub('<li>|</li>|<ul>|<i>|</i>|</ul>|</p>|<div.*?>|<br />', "", news)
                        overall_news += news
                if len(month_wise) > 0:
                    news_list[month_wise] = overall_news
