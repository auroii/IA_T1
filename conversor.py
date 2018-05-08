from PIL import Image
import numpy as np

im = Image.open("maze.bmp")
col,row = im.size
data=[]
pixels=im.load()
Matrix=[[0]*row]*col
i=o=p=j=0

img = open('grafo.txt','w')

for i in range(row):	
	for j in range(col):
		if(pixels[j,i][0]>=50 or pixels[j,i][1] >= 50 or pixels[j,i][2] >= 50):
			img.write('0')
		else:
			img.write('1')

		if(j==col-1):
            		img.write('\n')
		else:
			 img.write(' ')
    
img.close()
