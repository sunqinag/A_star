import numpy as np
import cv2
import threading

fx = 0.5
fy = 0.3

def write_to_file(text):  
    with open('./debug/node_list.txt', 'a') as file:  
        file.write(text + '\n') 

# 定义一个鼠标回调函数  
def mouse_click_event(event, x, y, flags, param):  
    if event == cv2.EVENT_LBUTTONDOWN:  
        # 在这里处理左键点击事件  
        print(f'Mouse position: ({x}, {y})-> ({int(x/fx)}, {int(y/fy)})/') 
        thread = threading.Thread(target=write_to_file, args=(f'{int(x/fx)}, {int(y/fy)}',))  
        thread.start()  

        # 如果你想在图像上标记点，可以取消注释以下代码  
        cv2.circle(image, (x, y), 5, (0, 255, 0), -1)  # 画一个绿色的圆  



if __name__ == '__main__':

    file = "/home/sunqiang/data/zz520/jueying.pgm"
    image  = cv2.imread(file,0)
    mask = np.zeros(image.shape)
    image  = cv2.resize(image,None,fx=fx,fy=fy)

    # mask_0 = image==254
    # mask[mask_0] = 255
    # cv2.imshow("0 value",mask)


    # 初始化一个窗口  
    window_name = 'Image Click'  

    # 绑定鼠标回调函数到窗口  
    cv2.namedWindow(window_name)  
    cv2.setMouseCallback(window_name, mouse_click_event)  

    # 显示图像  
    while True:  
        cv2.imshow(window_name, image)  
        if cv2.waitKey(1) & 0xFF == ord('q'):  # 如果按下'q'键，退出循环  
            break  
    
    cv2.destroyAllWindows()





#   0: obstacle
#   205: background
#   255: OK area


'''
446, 246,
1102, 1583,
1176, 1796,
1318, 1900,
1238, 1690,
1228, 2216,
1134, 2340,
1196, 2566,
1326, 2773,
1254, 2756,
1352, 2546,
1302, 2206,
1390, 1903,
1148, 2166,
1376, 2156,
444, 126
'''