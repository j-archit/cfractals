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

    new_image = Image.new("L", (i_o, j_o), color=0)
    drawer = ImageDraw.Draw(new_image)
    for line in f:
        i = 0
        sp = line.strip().split(" ")
        j = int(sp[0])
        for p in sp[1:]:
            i += 1
            drawer.point((int(p), j), fill=255)
        
        # Progress reported via each row
        if not j % 25:
            sl = (sl + 1) % 4
        print(f"\r   {sla[sl]} {(j*100/j_o):2.0f}%", end = "")

print("\n--------------------------");

try:
    subprocess.run("setterm --cursor on")
except:
    pass

print("   Saving Image:", end="")
new_image.save(f'{oname}.png')
print(f"\r   Done Saving:  \n   {oname}.bmp");
print("--------------------------");
