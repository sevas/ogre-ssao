#!/usr/bin/env python
# -*- coding: utf-8 -*-


from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from pprint import pprint



    


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

    ax.scatter(xs, ys, zs, s=50, c=make_colors(xs, ys, zs))
    plt.show()
    
    
    
    
if __name__ == '__main__':
    img_file = "regularSphereJittered4.png"
    img = Image.open(img_file)

    img_data = np.asarray(img)
    
    #plt.imshow(img_data, interpolation='nearest')
    #plt.show()
    
    
    img_data = (img_data.astype('float')) / 255

    dimensions = np.asarray([img_data[:,:,i] for i in (0,1,2)])
    

    xs, ys, zs = [arr.reshape((16)) for arr in dimensions]


    print "float3 sphereVectors[%d] { " % len(xs)
    for (x, y, z) in zip(xs, ys, zs):
        print "    float3(%f, %f, %f)," % (x,y,z)
    print "};"


    for (x, y, z) in zip(xs, ys, zs):
        print 
    
    make_scatterplot(xs, ys, zs)
