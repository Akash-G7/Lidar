import cv2
import numpy as np
from pyzbar.pyzbar import decode
 
cam = cv2.VideoCapture(0)

while True:
    success, img = cam.read()
    for qrcode in decode(img):
        print(qrcode.data)
        myData = qrcode.data.decode('utf-8')
        print(myData)
        pts = np.array([qrcode.polygon], np.int32)
        pts = pts.reshape((-1,1,2))
        cv2.polylines(img,[pts],True,(0,255,0),5)
        pts2 = qrcode.rect
        cv2.putText(img, myData, (pts2[0],pts2[1]),cv2.FONT_HERSHEY_SIMPLEX,0.9, (0,255,0),2)
    cv2.imshow('Result', img)
    cv2.waitKey(1)