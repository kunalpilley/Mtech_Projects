import requests

web_page_url = "https://www.worldometers.info/coronavirus/"
headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36'}

web_page = requests.get(url = web_page_url,headers=headers)
page_content = web_page.text
#print(page_content)

with open('mainpage.html','w',encoding='utf8') as file_pointer:
    file_pointer.write(page_content)
file_pointer.close

print('Saved web page content ' + web_page_url + ' successfully.')

main_url='https://www.worldometers.info/coronavirus/country/'

country_name=['France','UK','Russia','Italy','Germany','Spain','Poland','Netherlands','Ukraine','Belgium',
'US','Mexico','Canada','Cuba','Costa-Rica','Panama','India','Turkey','Iran','Indonesia','Philippines',
'Japan','Israel','Malaysia','Thailand','Viet-Nam','Iraq','Bangladesh','Pakistan','Brazil','Argentina','Colombia',
'Peru','Chile','Bolivia','Uruguay','Paraguay','Venezuela','South-Africa','Morocco','Tunisia','Ethiopia','Libya','Egypt','Kenya',
'Zambia','Algeria','Botswana','Nigeria','Zimbabwe','Australia','Fiji','Papua-New-Guinea','New-Caledonia',
'New-Zealand']

total_country = len(country_name)

#for i in range(0,total_country):
for i in range(total_country):
    try:
        country_url=main_url+country_name[i]
        webpage = requests.get(url = country_url,headers=headers)
    
        print('Web page saved for : ', i+1 ,' ',country_name[i] )
        page_content = web_page.text
        filename = country_name[i]+".html"
        with open(filename,'w',encoding='utf8') as file_pointer:
            file_pointer.write(page_content)
        file_pointer.close
    
    except:
            print('URL down for : ' + country_name[i] + ', couldn\'t save webpage.')
    
  