# -*- coding: utf-8 -*-
"""
Created on Sat Oct 27 21:57:44 2018

@author: yyhaos
"""

url='https://ibaotu.com/tupian/haibao.html'
# the target web, only google pics web is suitable
from pygame import init
from sys import exit
from pygame.display import set_mode
from pygame.display import set_caption
from pygame.transform import chop
from pygame.image import load
from pygame.image import save
from PIL import Image
#from pygame import QUIT
from pygame import quit
from pygame.transform import smoothscale
import os                    
import requests
from bs4 import BeautifulSoup as bs# need bs4
from urllib.request import Request,urlopen
maxn_x=600
maxn_y=900
half_y=455
init()
screen = set_mode((maxn_x,maxn_y))
set_caption("不用管")
os.makedirs('./image_baotu/', exist_ok=True)  #pics are saved in */image/

def pic_dl(IMAGE_URL,Name):
    try:
        r = requests.get(IMAGE_URL,timeout=5)
        with open(Name, 'wb') as f:
            f.write(r.content)  
    except:
        print('Can not save ',Name,' ' )
    
headers= {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36 Edge/15.15063'
    ,'Cookie':'Hm_lpvt_2b0a2664b82723809b19b4de393dde93=1541321771; Hm_lpvt_4df399c02bb6b34a5681f739d57787ee=1541321771; Array=%2F%2Fpic.ibaotu.com%2Fhead%2Fdefault.png; sajssdk_2015_cross_new_user=1; Hm_lvt_2b0a2664b82723809b19b4de393dde93=1541318577,1541318645,1541319027,1541319442; Hm_lvt_4df399c02bb6b34a5681f739d57787ee=1541318577,1541318645,1541319027,1541319442; FIRSTVISITED=1541317717.529; auth_id=18037103%7C188%2A%2A%2A%2A5161%7C1542614351%7Caa1b4296a020f8424ad369f3d8222309; sensorsdata2015jssdkcross=%7B%22distinct_id%22%3A%2218037103%22%2C%22%24device_id%22%3A%22166ddb1eb21628-09308141616f59-784a5037-1327104-166ddb1eb2266b%22%2C%22props%22%3A%7B%22%24latest_traffic_source_type%22%3A%22%E7%9B%B4%E6%8E%A5%E6%B5%81%E9%87%8F%22%2C%22%24latest_referrer%22%3A%22%22%2C%22%24latest_referrer_host%22%3A%22%22%2C%22%24latest_search_keyword%22%3A%22%E6%9C%AA%E5%8F%96%E5%88%B0%E5%80%BC_%E7%9B%B4%E6%8E%A5%E6%89%93%E5%BC%80%22%7D%2C%22first_id%22%3A%22166ddb1eb21628-09308141616f59-784a5037-1327104-166ddb1eb2266b%22%7D; bt_guid=dce9630b6aa3cdc1d0c3c24f521af77a; sign=PHONE; ISREQUEST=1; WEBPARAMS=is_pay=0; SERVERID=7e83eb899c176f8ef3b6e0b2538e64f5|1541321913|1541316531'
     }

def get_date(url):# from web get beautifulsoup
    request = Request(url,headers=headers)
    response = urlopen(request)
    print(response)
    html=bs(response)
    return html

def trans(file_name):
    image=load(file_name).convert_alpha()
    w,h=image.get_size()
    #print(w,h)
    #image=chop(image,(0,0,0,h-half_y))
    image = smoothscale(image,(maxn_x,maxn_y))
    save(image,file_name)
    
    return file_name
def link(file_name_1,file_name_2):
    try:
        image1=load(file_name_1).convert_alpha()
        image2=load(file_name_2).convert_alpha()
        w,h=image1.get_size()
        image1=chop(image1,(0,0,0,h-half_y))
        w,h=image1.get_size()
        w2,h2=image2.get_size()
        to_image=Image.new('RGB', (w,h+h2))
        save(image1,file_name_1)
       
        open_image=Image.open(file_name_1)
        to_image.paste(open_image,(0,0))
        
        open_image=Image.open(file_name_2)
        to_image.paste(open_image,(0,h))
        
        to_image.save(file_name_1)
        #image3=load(file_name_1).convert_alpha()
        trans(file_name_1)
        return 0
    except:
        print(file_name_1+' failed')
        return 1
cnt=1

test_url='https://ibaotu.com/?m=download&id=972325'
#html = get_date(test_url)
request = Request(test_url,headers=headers)
response = urlopen(request)
html = response.read().decode()
#print(html)
with open("rsp_cookie.html","w",encoding="utf-8")as f:
    # 将爬取的页面
    print(html)
    f.write(html)
'''
print('开始查找')
print(html.find_all('p'))
print('结束查找')
'''
'''
for page in range(1):
    if page==0:
        #continue
        url='https://ibaotu.com/tupian/haibao'+'.html'
    else:
        url='https://ibaotu.com/tupian/haibao/'+str(page)+'.html'
    
    html=get_date(url)
    tag=html.find_all('a',class_='jump-details jump-details-seo')
    #tag=html.find_all(class_="rg_meta notranslate")# this tag concludes the original weds of pics
    print('tag:')
    #print(tag)
    #cnt=1
    #Name='./image/img'+str(cnt)+'.png'
    sum=1
    for i in tag:
        if(sum<=0):
            break
        sum=sum-1
        print('---')
            #j_z=eval(j)
        print(i["seo-url"])
        image_url=i["seo-url"] # Original Website of pic
        image_url='https:'+image_url
        
        #print(Name)
        image_html=get_date(image_url)
        image_tag=image_html.find_all('div',class_='img-wrap')
        #print(image_tag)
        for j in image_tag:
            th=1
            name1='0'
            name2='0'
            for k in j:
                #print(k)
                
                if(th>=3):
                    Name='./image_baotu/img'+str(cnt)+'-'+str(th)+'.png'
                    if(th==3):
                        name1=Name
                    if(th==4):
                        name2=Name
                    image_url='https:'+k['data-url']
                    #print('https:'+k['data-url'])
                    pic_dl(image_url,Name)
                    #trans(Name)
                th=th+1
            print(name1,name2)
            link(name1,name2)
                
            cnt=cnt+1
                
            print('--')
        #pic_dl(image_url,Name)
        #cnt=cnt+1
'''
            
quit()
#exit()