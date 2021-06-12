from PIL import Image
import numpy as np
import sys

if len(sys.argv) < 3:
    print('Uso: [python] ./genfield.py <height> <out_image>')
    exit(-1)

exec(open(sys.argv[1]).read())

def normalize(arr, t_min, t_max): 
    norm_arr = [] 
    diff = t_max - t_min 
    min_arr = min(arr)
    max_arr = max(arr)
    diff_arr = max_arr - min_arr
    return (((np.array(arr) - min_arr)*diff)/diff_arr) + t_min

image = Image.new('L', (len(height[0]), len(height)))
array = normalize([p for row in height for p in row], 0, 256)

image.putdata(array)
image.save(sys.argv[2])
