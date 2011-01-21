#!/usr/bin/env python

from __future__ import division
import random as r
import math as m
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def norm(x, y, z):
    return m.sqrt(x*x + y*y + z*z)


def normalize(x, y, z):
    n = norm(x, y, z)
    return (x/n, y/n, z/n)


def normalize_and_scale(x, y, z):
    s = r.random()
    normalized = normalize(x, y, z)
    return (normalized[0]*s,
            normalized[1]*s,
            normalized[2]*s)



def make_colors(xs, yz, zs):
    def make_RBGA_string(x, y, z):
        return "#%02hx%02hx%02hx" % tuple([int(v*255) for v in (x, y, z)])
    return [make_RBGA_string(x, y, z) for (x, y, z) in zip(xs, ys, zs)]



def make_scatterplot(xs, ys, zs):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    ax.scatter(xs, ys, zs, s=50)#, c=make_colors(xs, ys, zs))
    plt.show()
    
    




if __name__ == '__main__':    
    N = 64

    xs, ys, zs = [], [], []

    for i in range(N):
        x =  r.uniform(0, 1) 
        y =  r.uniform(0, 1) 
        z =  r.uniform(0, 1) 

        print x, y, z
        sx, sy, sz = x, y, z #normalize_and_scale(x, y, z)
        xs.append(sx)
        ys.append(sy)
        zs.append(sz)
        
    
    make_scatterplot(xs, ys, xs)
