import sys
import os
import subprocess
from PIL import Image, ImageDraw

sdir = os.path.dirname(__file__) #<-- absolute dir the script is in

fname = os.path.join(sdir, "../output/default_c.txt")
argv = sys.argv[1:]
for i, arg in enumerate(argv):
    if arg == "-f":
        fname = os.path.join(sdir, argv[i+1])
    if arg == "-o":
        oname = os.path.join(sdir, argv[i+1]) 

with open(fname) as f:
    dim = f.readline().split(",")
    j_o = int(dim[1])
    i_o = int(dim[0])
    print("\n--------------------------");
    print("   Processing Output: ")
    count = 0
    sla = ['-', '/', '|', '\\', '-']
    try:
        subprocess.run("setterm --cursor off")
    except:
        pass
    sl = 0
    
    # { NEW APPROACH }
    new_image = Image.new("L", (i_o, j_o), color=0)
    drawer = ImageDraw.Draw(new_image)
    for j, line in enumerate(f):
        i = 0
        # k = 0;
        for b in line.strip().split(" "):
            i += 1
            fi = int(b) * 255
            if fi:
                drawer.point((i,j), fill=fi)
            # k = b;
        
        # Progress reported via each row
        if not j % 25:
            sl = (sl + 1) % 4
        print(f"\r   {sla[sl]} {(j*100/j_o):2.0f}%", end = "")
    # { NEW APPROACH }

    # { PREVIOUS APPROACH }
    # pixels = []
    # for line in f:
    #     pixels.append([int(n,base=16)*255 for n in line.strip().split(" ")])
    #     count += 1
    #     if not count % 8:
    #         sl = (sl + 1) % 4
    #     print(f"\r   {sla[sl]} {(count*100/j_o):2.1f}%", end = "")

print("\n--------------------------");

try:
    subprocess.run("setterm --cursor on")
except:
    pass

# { PREVIOUS APPROACH }
# import numpy as np
# Convert the pixels into an array using numpy
# print("   Processing Image")
# array = np.array(pixels, dtype=np.uint8)

# # Use PIL to create an image from the new array of pixels
# new_image = Image.fromarray(array)
# print("--------------------------");

print("   Saving Image:", end="")
new_image.save(f'{oname}.png')
print(f"\r   Done Saving:  \n   {oname}.bmp");
print("--------------------------");
