from numpy import append


all_headings = ["Total Cases : ", "Active Cases : ", "Total Deaths : ","Total Recovered: ","Total tests : ","Death / Million : "
,"Test / Million : ","New case : ","New Death : ","New Recovered : "]

world_data = dict.fromkeys(all_headings, None)
count = []
world = []
all_data = []
asia = []
asia_data = {}
north_america = []
north_america_data ={}
south_america = []
south_america_data ={}
europe = []
europe_data ={}
africa = []
africa_data = {}
ocenia = []
ocenia_data = {}

i = 0
day = 0



def t_CONTINENT(t):
    r'<nobr>[A-Za-z]+(\s)?[A-Za-z]*</nobr>\n</td>'
    return t

# def t_ALL(t):
#     #r'<td\sstyle="display:none" data-continent="all">All</td>'
#     r'<td></td>'

#     return t

# def t_OPEN(t):
#     r'<td>'
#     return t

# def t_CLOSE(t):
#     r'</td>'
#     return t


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


# def p_value(t):
#     'pvalue : VALUE'
#     t[0] = t[1] 
#     global day
#     print(" inside value : ", day)
#     if day == 2 :
#         print("VALUE : ", t[0])
#         count.append(t[0])
    #world_data[i] = t[0]

# def p_str(t):
#     'str : OPEN pvalue CLOSE '
#     t[0] = t[2]

# def p_multi_str(t):
#     'str : OPEN pvalue CLOSE str'
#     t[0] = t[2] + "| " + t[4]
#     #print("multi", t[0])


def p_Cont(t):
    'Cont : CONTINENT VALUE VALUE VALUE VALUE VALUE VALUE VALUE VALUE'
    global day
    day = day + 1
    # print("Cases :",day," value ", t[2],"-", t[3],"-", t[4],"-", t[5],"-", t[6],"-", t[7],"-", t[8],"-", t[9],"-")
    if day > 6 and day < 13:
        all_data.append(t[2]) # total cases
        all_data.append(t[8]) # active cases
        all_data.append(t[4]) # total death
        all_data.append(t[6]) # total recovered
        all_data.append(None) # total test
        all_data.append(None) # death PM
        all_data.append(None) # test PM
        all_data.append(t[3]) # new case 
        all_data.append(t[5]) # new death
        all_data.append(t[7]) # new recovered

def p_error(t):
    pass

import os
import re
import sys
import ply.lex as lex

tokens = [
    'CONTINENT',
    'VALUE'
    ] 
#movie = "Deadpool 2 (2018)"
text = open('mainpage.html', 'r', errors = 'ignore').read()

lexer = lex.lex()
lexer.input(str(text))
# for tok in lexer:
#      print(tok.type,tok.value)
    
    
import ply.yacc as yacc
parser = yacc.yacc()
parser.parse(text)

for i in range(len(all_data)):
    if i >=0 and i<=9 :
        asia.append(all_data[i])
    if i >=10 and i<=19 :
        north_america.append(all_data[i])
    if i >=20 and i<=29 :
        south_america.append(all_data[i])
    if i >=30 and i<=39 :
        europe.append(all_data[i])
    if i >=40 and i<=49 :
        africa.append(all_data[i])
    if i >=50 and i<=59 :
        ocenia.append(all_data[i])

asia_data = {all_headings[i]: asia[i] for i in range(len(asia))}
north_america_data = {all_headings[i]: north_america[i] for i in range(len(north_america))}
south_america_data = {all_headings[i]: south_america[i] for i in range(len(south_america))}
europe_data = {all_headings[i]: europe[i] for i in range(len(europe))}
africa_data = {all_headings[i]: africa[i] for i in range(len(africa))}
ocenia_data = {all_headings[i]: ocenia[i] for i in range(len(ocenia))}

def print_data(choice):

    if choice == '1':
        print("Asia\n")
        for i in asia_data:
            print(i,asia_data[i])
        print("\n")
        return asia_data

    elif choice == '2':
        print("North America\n")
        for i in north_america_data:
            print(i,north_america_data[i])
        print("\n")
        return north_america_data

    elif choice == '3':
        print("South America\n")
        for i in south_america_data:
            print(i,south_america_data[i])
        print("\n")
        return south_america_data

    elif choice == '4':
        print("Europe\n")
        for i in europe_data:
            print(i,europe_data[i])
        print("\n")
        return europe_data

    elif choice == '5':
        print("Africa\n")
        for i in africa_data:
            print(i,africa_data[i])
        print("\n")
        return africa_data

    elif choice == '6':
        print("Ocenia\n")
        for i in ocenia_data:
            print(i,ocenia_data[i])
        print("\n")
        return ocenia_data

    else:
        print("\nwrong input :", choice," Enter value between 1- 6\n")
        empty_dictionary = {}
        return