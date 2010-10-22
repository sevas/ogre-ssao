#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = "Frederic De Groef (f.degroef@gmail.com)"
__date__   = "Fri Oct 22 14:25:32 2010"


from PIL import Image
import os
import numpy as np
import matplotlib.pyplot as plt
from math import sqrt, cos, asin, pi

MEDIA = "data/"
DEPTH_BUFFER = MEDIA + "0_1280_800_zbuf.dat"
NORMAL_BUFFER = MEDIA + "0_1280_800_normals.dat"



def extract_size_from_name(filename):
    """
    
    Paramters:
    - `filename`: 
    """
    w, h = [int(n) for n in filename.split("/")[1].split("_")[1:3]]
    return (w, h)



def open_depth_buffer(filename):
    """
    
    Paramters:
    - `filename`: 
    """
    w, h = extract_size_from_name(filename)
    f = open(filename, "rb")
    buf = f.read(w*h*4)
    f.close()
    img = np.frombuffer(buf, dtype=np.float32)
    img = img.reshape((h, w))
    return img


def open_normals_buffer(filename):
    """
    
    Paramters:
    - `filename`: 
    """
    w, h = extract_size_from_name(filename)
    f = open(filename, "rb")
    buf = f.read(w*h*4*4)
    f.close()
    #img = np.frombuffer(buf, dtype=np.dtype([('r', "f4"), ('g', "f4"), ('b', "f4"), ('a', "f4")]))
    img = np.frombuffer(buf, dtype="f4<")
    img = img.reshape((h, w, 4))
    return img





def tex2D(array, uv):
    u, v = uv
    w, h = array.shape[:2]
    return array[u*w, v*h]

def setValue(array, uv, val):
    u, v = uv
    w, h = array.shape[:2]
    array[u*w, v*h] = val



def dot_product(v1, v2):
    return sum([v1[i]*v2[i] for i in (0, 1, 2)]) 


def length(v1):
    return sqrt(sum([v[i]**2 for i in (0, 1, 2)]))


def normalize(v1):
    norm = length(v1)
    return tuple([v[i]/norm for i in (0, 1, 2)])


def make_vector_from_points(x, y):
    return tuple([y[i] - x[i] for i in (0, 1, 2)]) 


def compute_h(P, C, r):
    PC = make_vector_from_points(P, C)
    return 1.0 - cos(asin(r/length(PC)))

def sphere_cap_area(P,C, r):
    h = compute_h(P, C, r)
    return 2*pi*h


def compute_approximated_AO(C, r, P, n):
    PC_dir = normalize(make_vector_from_points(P, C))
    return sphere_cap_area(P, C, r) * max(dot_product(n, PC_dir), 0.0)




def make_sphere(depth):
    """
    """
    pass




def compute_ambient_occlusion(x, y, depth, normals):
    """
    
    Paramters:
    - `i`: 
    - `j`: 
    - `depth`: 
    - `normals`: 
    """
    z = tex2D(depth, (x, y))
    n = tex2D(normals, (x, y))
    
    occlusion = 0.0
    for i in range(10):
        Qi, ri = make_sphere(i)
        PQi = make_vector_from_points(P, Qi)
        if length(PQi) < r_far: 
            occlusion += compute_approximated_AO(Qi, ri, P, n)

    return occlusion
        

    

    


if __name__ == '__main__':
    depth = open_depth_buffer(DEPTH_BUFFER)
    normals = open_normals_buffer(NORMAL_BUFFER)

    print normals.shape, depth.shape


    h, w = depth.shape
    ao = np.zeros((h, w), dtype="float")


    for u in [float(x)/1000 for x in range(1000)]:
        for v in [float(x)/1000 for x in range(1000)]:
            occlusion = compute_ambient_occlusion(u, v, depth, normals)
            setValue(ao, (u, v), occlusion)


    plt.imshow(ao)
    plt.show()
