# -*- coding: utf-8 -*-
"""
Created on Wed Nov  7 20:31:27 2018

@author: HASEE
"""

import os
#path=input('请输入文件路径(结尾加上/)：')       
path='C:\\Users\\HASEE\\Desktop\\本基\\Dateset\\1\\'
path2='C:\\Users\\HASEE\\Desktop\\本基\\Dateset\\2\\'

#获取该目录下所有文件，存入列表中
f=os.listdir(path)

n=0
for i in f:
    
    #设置旧文件名（就是路径+文件名）
    oldname=path+f[n]
    
    #设置新文件名
    try:
        newname=path+str(n+1)+'.png'
    
    #用os模块中的rename方法对文件改名
        os.rename(oldname,newname)
        print(oldname,'======>',newname)
    except:
        print(newname+' already renamed')
    n+=1
    
#获取该目录下所有文件，存入列表中
f=os.listdir(path2)

n=0
for i in f:
    
    #设置旧文件名（就是路径+文件名）
    oldname=path2+f[n]
    
    #设置新文件名
    try:
        newname=path2+str(n+1)+'.png'
    
    #用os模块中的rename方法对文件改名
        os.rename(oldname,newname)
        print(oldname,'======>',newname)
    except:
        print(newname+' already renamed')
    
    n+=1