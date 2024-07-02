import cv2
import numpy as np
from tqdm import tqdm

if __name__ == '__main__':
    
    file = "/home/sunqiang/data/zz520/jueying.pgm"
    image  = cv2.imread(file)

    path_file = './debug/path.txt'
    node_file = "./debug/node_list.txt"
    path = np.loadtxt(path_file,delimiter=',')
    nodes = np.loadtxt(node_file,delimiter=',')
    for i, node in enumerate(nodes):
        cv2.circle(image, node.astype(np.int32), 4, (255,0,0))
        cv2.putText(image, str(i), node.astype(np.int32)+3, cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,0,0), 2)

    for i, point in enumerate(tqdm(path)):
        cv2.circle(image,point.astype(np.int32),2,(0,0,255))
        cv2.imwrite('./debug/images/'+str(i)+".png",image)
    