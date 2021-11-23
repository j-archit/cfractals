import sys
import os
import subprocess
from PIL import Image, ImageDraw

sdir = os.path.dirname(__file__) #<-- absolute dir the script is in
sdir = os.path.dirname(sdir)
sdir = os.path.abspath(sdir)

def_fname = os.path.join("output", "default_c.txt")
fname = os.path.join(sdir, def_fname)

argv = sys.argv[1:]
for i, arg in enumerate(argv):
    if arg == "-f":
        fname = os.path.abspath(argv[i+1])
    if arg == "-o":
        oname = os.path.abspath(argv[i+1]) 

def color_lookup(a):
    return round(240*(a/7)**2)

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

    new_image = Image.new("L", (i_o, j_o), color=0)
    drawer = ImageDraw.Draw(new_image)
    for line in f:
        sp = line.strip().split(" ")
        row = int(sp[0])
        pix = sp[1:]
        
        # Monotone
        # i = 0
        # while True:
        #     try:
        #         start = int(pix[i])
        #     except IndexError:
        #         break
        #     try:
        #         end = int(pix[i+1])
        #     except IndexError:
        #         end = i_o
        #     i += 2

        #     for pixeli in range(start, end):
        #         drawer.point((pixeli, row), fill=255)
            
        #     if end == i_o:
        #         break

        # Color Layers
        i = 0
        color = 0
        while True:
            try:
                color, start = pix[i].split(":")
                color = int(color)
                start = int(start)
            except IndexError:
                break
            try:
                c , end = pix[i+1].split(":")
                end = int(end)
            except IndexError:
                end = i_o
            i += 1

            for pixeli in range(start, end):
                drawer.point((pixeli, row), fill=color_lookup(color))
            
            if end == i_o:
                break
        
        # Progress reported via each row
        if not row % 25:
            sl = (sl + 1) % 4
        print(f"\r   {sla[sl]} {(row*100/j_o):2.0f}%", end = "")

print("\n--------------------------");

try:
    subprocess.run("setterm --cursor on")
except:
    pass

print("   Saving Image:", end="")

if 'oname' not in locals():
    oname = os.path.join(sdir, "output")

new_image.save(f'{oname}_{i_o}_{j_o}.png')
print(f"\r   Done Saving:  \n   {oname}_{i_o}_{j_o}.png");
print("--------------------------");
