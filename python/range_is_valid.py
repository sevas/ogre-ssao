

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from pprint import pprint
from scipy.weave import inline, converters




def saturate(xs, ys):
    num_samples = xs.shape[0]
    zs = np.zeros((num_samples, num_samples))

    print zs
    
    code = """

    for (int i=0; i<num_samples; ++i)
    {
        float frag = xs(i);
        for (int j=0; j<num_samples; ++j)
        {
            float sample = ys(j);

            float val = (frag - sample) / RADIUS;
            
            if(val < 0.0f)
                val = 0.0f;
            else if(val > 1.0f)
                val = 1.0f;
            zs(i, j) = val;
        }
    }

    
    """

    RADIUS =  50
    
    inline(code, ['xs', 'ys', 'zs', 'RADIUS', 'num_samples'],
           type_converters=converters.blitz,
           compiler = 'gcc',
           verbose = 1)

    return zs




if __name__ == '__main__':
    near_clip_distance = 0.05
    far_clip_distance = 200

    num_samples = 100
    xs = np.linspace(near_clip_distance, far_clip_distance, num_samples)
    ys = np.linspace(near_clip_distance, far_clip_distance, num_samples)

    zs = saturate(xs, ys)

    print zs

    
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel('frag')
    ax.set_ylabel('sample')
    ax.set_zlabel('saturate')


    X, Y = np.meshgrid(xs, ys)
    #x.plot_surface(X, Y, zs)
    ax.scatter(xs, ys, zs)

    
    plt.show()
    print xs
    print ys
    print zs
    
