# -*- coding: utf-8 -*-
"""
Created on Sat Oct 27 21:57:44 2018

@author: HASEE
"""

import os                    
import requests
os.makedirs('./image/', exist_ok=True)

def pic_dl(IMAGE_URL,Name):
    try:
        r = requests.get(IMAGE_URL,timeout=5)
        with open(Name, 'wb') as f:
            f.write(r.content)  
    except:
        print('Can not save ',Name,' ' )
    
from bs4 import BeautifulSoup as bs
from urllib.request import Request,urlopen
headers= {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36 Edge/15.15063'
    }
def get_date(url):
    #url = 'https://www.baidu.com/'
    request = Request(url,headers=headers)
    response = urlopen(request)
    print(response)
    html=bs(response)
    return html

url='https://www.google.com.hk/search?newwindow=1&safe=strict&as_st=y&hl=zh-CN&tbs=isz%3Alt%2Cislt%3Avga&tbm=isch&sa=1&ei=hZTUW4CeO8if0gLJsoyoDw&q=school+poster&oq=school+poster&gs_l=img.3..0i10i19k1j0i7i30i19k1l9.13967.16486.0.16702.8.8.0.0.0.0.434.1235.2-3j0j1.4.0....0...1c.1j4.64.img..4.3.799...0i7i30k1.0.Yegt4da9gMU'

html=get_date(url)
tag=html.find_all('a',class_='rg_l')
tag=html.find_all(class_="rg_meta notranslate")#爬取的html文件和浏览器里的不一样
print('tag:')
#print(tag)
cnt=1
Name='./image/img'+str(cnt)+'.png'
for i in tag:
    print('---')
    for j in i:
        j_z=eval(j)
        print(j_z["ou"])
        emage_url=j_z["ou"]
        Name='./image/img'+str(cnt)+'.png'
        print(Name)
        pic_dl(emage_url,Name)
        cnt=cnt+1
        
        
    