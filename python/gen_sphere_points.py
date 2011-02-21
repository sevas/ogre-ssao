#!/usr/bin/env python

from __future__ import division
import random as rnd
import math as m
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


TAU = 2 * m.pi


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
    
    



def make_random_sphere_point():
    x =  rnd.uniform(0, 1) 
    y =  rnd.uniform(0, 1) 
    z =  rnd.uniform(0, 1) 
    return x, y, z
    

def make_random_polar_coords():
    radius = rnd.uniform(0, 1)
    theta = rnd.uniform(0, TAU)
    phi = rnd.uniform(0, TAU)

    return radius, theta, phi


def polar_to_euclidean(radius, theta, phi):
    x = radius * m.sin(theta) * m.cos(phi)
    y = radius * m.sin(theta) * m.sin(phi)
    z = radius * m.cos(theta)
    return x, y, z




def scale(x, y, z):
    s = 1 - (rnd.uniform(0, 1))**2
    return (x*s,
            y*s,
            z*s)


def sphere_picking():
    phi = rnd.uniform(0, TAU)
    u = m.cos(phi)
    theta = rnd.uniform(0, TAU)
    x = m.sqrt(1 - u**2) * m.cos(theta)
    y = m.sqrt(1 - u**2) * m.sin(theta)
    z = u
    return x, y, z
    
if __name__ == '__main__':    
    N = 1000

    xs, ys, zs = [], [], []

    print "float3 spherePoints[%d] { " % N
    
    for i in range(N):
        #r, theta, phi = make_random_polar_coords()
        #x, y, z = polar_to_euclidean(1, theta, phi)
        
        x, y, z = scale(*sphere_picking())
        print "\tfloat3(%f, %f, %f)," % (x, y, z)

        xs.append(x)
        ys.append(y)
        zs.append(z)


    #for i in range(int(N/2)):
    #    r, theta, phi = make_random_polar_coords()
    #    x, y, z = polar_to_euclidean(r, theta, phi)
    #    #x, y, z = make_random_sphere_point()
    #    print "\tfloat3(%f, %f, %f)," % (x, y, z)
    #    xs.append(x)
    #    ys.append(y)
    #    zs.append(z)

    print "};"
    make_scatterplot(xs, ys, zs)

    
