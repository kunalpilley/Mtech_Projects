from itertools import count

from pandas import notnull
import ply.lex as lex
import ply.yacc as yacc
import re
import requests
import sys

all_headings = ["Total Cases : ", "Active Cases : ", "Total Deaths : ","Total Recovered: ","Total tests : ","Death / Million : "
,"Test / Million : ","New case : ","New Death : ","New Recovered : "]

def comparision_continent(continent_dict, world_dict):
    for key in continent_dict:
        if continent_dict[key] is not None:
            continent_dict[key] = continent_dict[key].replace(",", "")
    
    for key in world_dict:
        if world_dict[key] is not None:
            world_dict[key] = world_dict[key].replace(",", "")


    print("\n\nThe percentage of total world population is as follows:\n\n")
    print("Total Cases : ",round(((float(continent_dict["Total Cases : "])/float(world_dict["Total Cases : "]))*100),2))
    print("Active Cases : ",round(((float(continent_dict["Active Cases : "])/float(world_dict["Active Cases : "]))*100),2))
    print("Total Deaths : ",  round(((float(continent_dict["Total Deaths : "])/float(world_dict["Total Deaths : "]))*100),2)   )
    print("Total Recovered: ",round(((float(continent_dict["Total Recovered: "])/float(world_dict["Total Recovered: "]))*100),2))
    print("Total tests : Insufficient Data",)
    print("Death / Million : Insufficient Data")
    print("Test / Million : Insufficient Data")
    print("New case : ",round(((float(continent_dict["New case : "])/float(world_dict["New case : "]))*100),2))
    print("New Death : ", round(((float(continent_dict["New Death : "])/float(world_dict["New Death : "]))*100),2) )
    print("New Recovered : ", round(((float(continent_dict["New Recovered : "])/float(world_dict["New Recovered : "]))*100),2)    )

def comparision_country(country_dict, world_dict):
    for key in country_dict:
        if country_dict[key] is not None:
            country_dict[key] = country_dict[key].replace(",", "")
    
    for key in world_dict:
        if world_dict[key] is not None:
            world_dict[key] = world_dict[key].replace(",", "")

    print("\n\nThe percentage of total world population is as follows:\n\n")
    print("Total Cases : ", round(((float(country_dict["Total Cases : "])/float(world_dict["Total Cases : "]))*100),2)      )
    print("Active Cases : ",round(((float(country_dict["Active Cases : "])/float(world_dict["Active Cases : "]))*100),2)    )
    print("Total Deaths : ", round(((float(country_dict["Total Deaths : "])/float(world_dict["Total Deaths : "]))*100 ),2)   )
    print("Total Recovered: ", round(((float(country_dict["Total Recovered: "])/float(world_dict["Total Recovered: "]))*100),2)    )
    print("Total tests : Insufficient Data",)
    print("Death / Million : ", round(((float(country_dict["Death / Million : "])/float(world_dict["Death / Million : "]))*100),2)  )
    print("Test / Million : Insufficient Data")
    print("New case : ", round(((float(country_dict["New case : "])/float(world_dict["New case : "]))*100),2)    )
    print("New Death : ",round(((float(country_dict["New Death : "])/float(world_dict["New Death : "]))*100),2)  )
    print("New Recovered : ",round(((float(country_dict["New Recovered : "])/float(world_dict["New Recovered : "]))*100),2)  )


    


print("\nSaving the Main Webpage and Specified Counties' Webpages as per Task 1\n")
import geturl1
print("\nFetching World Data to be used in Task 2.... \n")
print("\nThis will take about 5 minuts. Kindly wait\n")
import world1 as world

while(1):
    print(''' 
    \n
*******************
    MAIN MENU   
Enter your choice :

1 for World Data
2 for Continent Data
3 for Country Data
4 for Percentage Change for Specific Country
5 for Exit

********************
    ''')

    choice  = input()

    if choice == '1':
        print("Fetching world data .....Please wait\n")
        print("Printing World Data\n")
        world.print_data()

    elif choice == '2':
        print(''' 
********************************

Enter your choice for Continent
1 for Asia
2 for North America
3 for South America
4 for Europe
5 for Africa
6 for Ocenia
7 for MAIN MENU of task 4

*********************************
        \n
        ''')

        while(1):
            choice1 = input()
            c = int(choice1)
            if c == 7:
                print("Returing to Main Menu\n")
                break

            elif c <1 or c >6:
                print("Invalid Input for Continent!! Please enter Choice Again\n")
                continue
            else:
                print("\nFetching Continent data .....Please wait\n")
                print("This will take about 5 minuts. Kindly wait\n")
                import continentfinal as continent
                print("\nPrinting Continent Data \n")
                temp_continent = {}
                temp_continent = continent.print_data(choice1) 
                comparision_continent(temp_continent,world.world_data)
                break

    elif choice == '3':
        print("\nProcessing Country Data\n")
        import country
        comparision_country(country.country_data,world.world_data)
        del sys.modules['country']

    elif choice == '4':
         print("\nModule for Task 4\n")
         import date_wise
         
         del sys.modules['date_wise']
         
    
    elif choice == '5':
         print("\nTerminating Operations!!!\n")
         break
    
    else:
        print("\nInvalid Input !! Please Try Again\n")

