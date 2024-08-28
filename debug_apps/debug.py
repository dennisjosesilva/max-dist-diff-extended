import morphotreepy as mt 
import numpy as np 
import imageio.v2 as im 


f = im.imread("../dat/lena.pgm")
domain = mt.Box.fromSize(mt.UI32Point(f.shape[1], f.shape[0]))
fv = mt.UI8Vector(f.ravel())

tree = mt.UI8buildMaxTree(fv, mt.Adjacency8C(domain))

nodeImg = tree.reconstructNodeNumpy(4573, domain)
im.imwrite("4573.png", (1 - nodeImg) * 255)

nodeImg = tree.reconstructNodeNumpy(4514, domain)
im.imwrite("4514.png", (1 - nodeImg) * 255)