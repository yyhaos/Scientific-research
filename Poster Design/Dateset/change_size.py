# -*- coding: utf-8 -*-
"""
Created on Wed Nov  7 20:31:27 2018

@author: HASEE
"""

#path=input('请输入文件路径(结尾加上/)：')       
path= r"C:\Users\HASEE\Desktop\research\Dataset\2.26POS\*"
path2=r"C:\Users\HASEE\Desktop\research\Dataset\2.26NEG\*"
savepath=r"C:\Users\HASEE\Desktop\research\Dataset\2.26\POS_TRAIN"
savepath2=r"C:\Users\HASEE\Desktop\research\Dataset\2.26\NEG_TRAIN"


#提取目录下所有图片,更改尺寸后保存到另一目录

from PIL import Image

import os.path
import os
import numpy
import glob

def convertjpg(jpgfile,outdir,width=120,height=180):
    
    img=Image.open(jpgfile)

    try:
        bw_img=img.resize((width,height),Image.BILINEAR)   
        #bw_img=img.convert('L')
        new_img=bw_img.convert("RGB")
        #new_img=img.convert("PNG")
        #print(1)
        
        #os.makedirs(os.path.join(outdir,os.path.basename(jpgfile)))
        #new_p = Image.fromarray(new_img)
        #if new_p.mode != 'PNG':
        #    new_p = new_p.convert('PNG')
        new_img.save(os.path.join(outdir,os.path.basename(jpgfile)))

    except Exception as e:

        print(e)
        #print(jpgfile)
try:
    os.makedirs(savepath) 
except:
    print(savepath+" path already exits")

try:
    os.makedirs(savepath2) 
except:
    print(savepath2+" path already exits")

    
for jpgfile in glob.glob(path):
    convertjpg(jpgfile,savepath)
    print(jpgfile)
for jpgfile in glob.glob(path2):
    convertjpg(jpgfile,savepath2)
    print(jpgfile)
