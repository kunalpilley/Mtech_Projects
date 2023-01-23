all_headings = ["Total Cases : ", "Active Cases : ", "Total Deaths : ","Total Recovered: ","Total tests : ","Death / Million : "
,"Test / Million : ","New case : ","New Death : ","New Recovered : "]

world_data = dict.fromkeys(all_headings, None)
count = []
i = 0
day = 1
def t_WORLD(t):
    #r'<td style="text-align:left;">World</td>' <td\s(a-z-:;\")+>World</td>
    r'<td\sstyle="text-align:left;">World</td>'
    return t

def t_ALL(t):
    #r'<td\sstyle="display:none" data-continent="all">All</td>'
    r'<td></td>'

    return t

def t_OPEN(t):
    r'<td>'
    return t

def t_CLOSE(t):
    r'</td>'
    return t

def t_VALUE(t):
    # r"[A-Za-z0-9':.\-()]+"
    #r'((\d+)(,\d+)*)'
    r'\+?\d+[\d+,.]+'
    return t

t_ignore = " \t\n"

def t_error(t):
        t.lexer.skip(1)


def p_start(t):
    '''start : world
             '''


def p_value(t):
    'pvalue : VALUE'
    t[0] = t[1] 
    global day
    if day == 2 :
        # print("VALUE : ", t[0])
        count.append(t[0])
    #world_data[i] = t[0]



def p_str(t):
    'str : OPEN pvalue CLOSE '
    t[0] = t[2]

def p_multi_str(t):
    'str : OPEN pvalue CLOSE str'
    t[0] = t[2] + "| " + t[4]
    #print("multi", t[0])

def p_fstr(t):
    'world : WORLD str ALL'
    global day
    day = day + 1
    # if day == 3 :
    #     print("world data string : ", t[2])



def p_error(t):
    pass

import os
import re
import sys
import ply.lex as lex

tokens = [
    'WORLD',
    'ALL',
    'OPEN',
    'CLOSE',
    'VALUE'
    ] 
#movie = "Deadpool 2 (2018)"
text = open('mainpage.html', 'r', errors = 'ignore').read()

lexer = lex.lex()
lexer.input(str(text))
# for tok in lexer:
#     print(tok.type,tok.value)
    
    
import ply.yacc as yacc
parser = yacc.yacc()
parser.parse(text)

# for i in range(len(count)):
#     print(count[i])

world_data["Total Cases : "] = count[0]
world_data["Active Cases : "] = count[6]
world_data["Total Deaths : "] = count[2]
world_data["Total Recovered: "] = count[4]
#world_data["Total tests : "] = count[] - values not available
world_data["Death / Million : "] = count[9]
#world_data["Test / Million : "] =  values not available
world_data["New case : "] = count[1]
world_data["New Death : "] = count[3]
world_data["New Recovered : "] = count[5]

def print_data():
    print("\n*********** world details **********\n")
    for i in world_data:
        print(i,world_data[i])
    print("\n*********** world details end *******\n")


