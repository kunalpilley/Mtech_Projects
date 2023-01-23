from itertools import count
import ply.lex as lex
import ply.yacc as yacc
import re


all_headings = ["Active Cases : ", "Daily Death : ", "New Recovered : ","New Cases: "]
day = 0

country_data = dict.fromkeys(all_headings, None)
data1 = dict.fromkeys(all_headings, None)
data2 = dict.fromkeys(all_headings, None)
country_name = ''
dates = []
total_cases = []
daily_new_cases = []
active_cases = []
daily_deaths = []
new_recoveries = []

print('''

Enter Choice for Country as Number Shown Against:

1: France , 2: UK , 3: Russia , 4: Italy, 5: Germany
6: Spain , 7: Poland , 8: Netherlands , 9: Ukraine, 10: Belgium
11: US , 12: Mexico ,13: Canada , 14: Cuba , 15: Costa-Rica
16: Panama , 17 : India , 18: Turkey , 19 : Iran , 20 : Indonesia
21 : Philippines , 22 : Japan , 23 : Israel , 24 : Malaysia , 25 : Thailand
26 : Viet-Nam, 27 : Iraq , 28 : Bangladesh ,29 : Pakistan , 30 : Brazil
31 : Argentina , 32: Colombia , 33: Peru , 34 : Chile , 35 : Bolivia
36 : Uruguay , 37 : Paraguay , 38 : Venezuela, 39 : South-Africa, 40 : Morocco
41 : Tunisia , 42 : Ethiopia , 43 : Libya  , 44 : Zambia , 45 :Algeria
46 : Botswana , 47 : Nigeria , 48 : Zimbabwe , 49 : Australia , 50 : Fiji
51 : Papua-New-Guinea , 52 : New-Caledonia , 53 : New-Zealand , 54 : Egypt , 55 : Kenya
''')
while(1):
    ch = input()
    try:
        int(ch)
    except ValueError:
        print ("Not a integer: invalid choice try again")
        continue

    c = int(ch)    
    if c >=1 and c<=55:
        break
    else:
        print("invalid choice: Out of range ! try again")


Name_list = ['France','UK','Russia','Italy','Germany',
'Spain','Poland','Netherlands','Ukraine','Belgium',
'USA','Mexico','Canada','Cuba','Costa\sRica',
'Panama','India','Turkey','Iran','Indonesia',
'Philippines','Japan','Israel','Malaysia','Thailand',
'Vietnam','Iraq','Bangladesh','Pakistan','Brazil',
'Argentina','Colombia','Peru','Chile','Bolivia',
'Uruguay','Paraguay','Venezuela','South\sAfrica','Morocco',
'Tunisia','Ethiopia','Libya','Zambia','Algeria',
'Botswana','Nigeria','Zimbabwe','Australia','Fiji',
'Papua\sNew\sGuinea','New\sCaledonia','New\sZealand','Egypt','Kenya']

print(Name_list[c-1])

name = Name_list[c-1]

date1 = input("enter the date 1 in the format of Mmm DD, YYYY.Example, Mar 08, 2022 \n")
date2 = input("enter the date 2 in the format of Mmm DD, YYYY.Example, Mar 08, 2022 \n")

print("This will take about 5-7 minuts. Kindly wait\n")

tokens = ['HEAD','VALUE']

t_HEAD = r'<td\s[a-z0-9\s\n-:=;\"]+><a\s[a-z\s;:=\"/\_]+>{}<\/a><\/td>'.format(
    name)

def t_VALUE(t):
    # r"[A-Za-z0-9':.\-()]+"
    #r'((\d+)(,\d+)*)'
    r'(\+?\d+[\d+,.]+)|(<td></td>)'
    return t

t_ignore = " \t"

def t_error(t):
        t.lexer.skip(1)


def p_start(t):
    '''start : Cont 
             '''

def p_Cont(t):
    'Cont : HEAD VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE '
    #       1       2   3       4   5       6   7       8   9       10  11     12   13     14   
    # all_headings = [" 2 Total Cases :", " 3 New Cases :", " 4Total Deaths :","5 New Deaths :"," 6Total Recovered : "
    # ,7 garbage " 8 New Recovered"," 9 Active Cases :"," 10 Serious/Critical :"," 11 tot case Pm",
    # "12 death pm :","13 total test :","14 Test PM","15 Population

    global day
    day = day + 1
   # print("Cases :",day," value ", t[2]," ", t[3]," ", t[4]," ", t[5]," ", t[6]," ", t[7], " ",t[8]," ", t[9]," ", t[10]," ",
    # t[11]," ",t[12]," ", t[13]," ",t[14])

    if day == 2:
       data1["Active Cases : "] = t[2]
       data1["Daily Death : "]= t[9]
       data1["New Recovered : "] = t[4]
       data1["New Cases: "] = t[6]

       data2["Active Cases : "] = t[13]
       data2["Daily Death : "] = t[12]
       data2["New Recovered : "] = t[14]
       data2["New Cases: "] = t[3]
     
def p_error(t):
    pass


text = open('mainpage.html', 'r', errors = 'ignore').read()
lexer = lex.lex()
lexer.input(str(text))
# for tok in lexer:
#      print(tok.type,tok.value)   
import ply.yacc as yacc
parser = yacc.yacc()
parser.parse(text)   


print("Data parsed but not processes , Incomplete Module")
print("\n Data parsed but calculations not implemented \n")
print("\n Module Incomplete.... Returing to main \n")
