# -*- coding: utf-8 -*-
"""
Created on Sat Oct 27 21:57:44 2018

@author: yyhaos
"""

url='https://www.google.com.hk/search?newwindow=1&safe=strict&as_st=y&hl=zh-CN&tbs=isz%3Alt%2Cislt%3Avga&tbm=isch&sa=1&ei=hZTUW4CeO8if0gLJsoyoDw&q=school+poster&oq=school+poster&gs_l=img.3..0i10i19k1j0i7i30i19k1l9.13967.16486.0.16702.8.8.0.0.0.0.434.1235.2-3j0j1.4.0....0...1c.1j4.64.img..4.3.799...0i7i30k1.0.Yegt4da9gMU'
# the target web, only google pics web is suitable

import os                    
import requests
from bs4 import BeautifulSoup as bs# need bs4
from urllib.request import Request,urlopen

os.makedirs('./image/', exist_ok=True)  #pics are saved in */image/

def pic_dl(IMAGE_URL,Name):
    try:
        r = requests.get(IMAGE_URL,timeout=5)
        with open(Name, 'wb') as f:
            f.write(r.content)  
    except:
        print('Can not save ',Name,' ' )
    
headers= {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36 Edge/15.15063'
    }

def get_date(url):# from web get beautifulsoup
    request = Request(url,headers=headers)
    response = urlopen(request)
    print(response)
    html=bs(response)
    return html

html=get_date(url)
tag=html.find_all('a',class_='rg_l')
tag=html.find_all(class_="rg_meta notranslate")# this tag concludes the original weds of pics
print('tag:')
#print(tag)
cnt=1
Name='./image/img'+str(cnt)+'.png'
for i in tag:
    print('---')
    for j in i:
        j_z=eval(j)
        print(j_z["ou"])
        image_url=j_z["ou"] # Original Website of pic
        Name='./image/img'+str(cnt)+'.png'
        print(Name)
        pic_dl(image_url,Name)
        cnt=cnt+1
        
        
    