import serial
import binascii
from CalcLidarData import CalcLidarData
from cvzone.SerialModule import SerialObject
import matplotlib.pyplot as plt
import math
import cv2
import numpy as np
import time
from pyzbar.pyzbar import decode
 
#cam = cv2.VideoCapture(0)
arduino = SerialObject()

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111, projection='polar')
ax.set_title('lidar (exit: Key E)',fontsize=18)

# Eキーを押すと終了します。
plt.connect('key_press_event', lambda event: exit(1) if event.key == 'e' else None)

ser = serial.Serial(port='COM7',
                    baudrate=230400,
                    timeout=5.0,
                    bytesize=8,
                    parity='N',
                    stopbits=1)

tmpString = ""
lines = list()
angles = list()
distances = list()
i = 0
while True:
    loopFlag = True
    flag2c = False

   #success, img = cam.read()
   #for qrcode in decode(img):
   #     print(qrcode.data)
   #     myData = qrcode.data.decode('utf-8')
   #     print(myData)
   #     pts = np.array([qrcode.polygon], np.int32)
   #     cv2.polylines(img,[pts],True,(0,255,0),5)
   #     pts2 = qrcode.rect
   #     cv2.putText(img, myData, (pts2[0],pts2[1]),cv2.FONT_HERSHEY_SIMPLEX,0.9, (0,255,0),2)
   # cv2.imshow('Result', img)
   # cv2.waitKey(1)
    if(i % 40 == 39):
        if('line' in locals()):
            line.remove()
        line = ax.scatter(angles, distances, c="pink", s=5)

        ax.set_theta_offset(math.pi / 2)
        plt.pause(0.01)
        angles.clear()
        distances.clear()
        i = 0
        

    while loopFlag:
        print("hi")
        b = ser.read()
        tmpInt = int.from_bytes(b, 'big')
        
        if (tmpInt == 0x54):
            tmpString +=  b.hex()+" "
            flag2c = True
            continue
        
        elif(tmpInt == 0x2c and flag2c):
            tmpString += b.hex()

            if(not len(tmpString[0:-5].replace(' ','')) == 90 ):
                tmpString = ""
                loopFlag = False
                flag2c = False
                continue

            lidarData = CalcLidarData(tmpString[0:-5])
            angles.extend(lidarData.Angle_i)
            distances.extend(lidarData.Distance_i)
            if (lidarData.Angle_i[1] < 1 or lidarData.Angle_i[1] > 5) and lidarData.Distance_i[1] <= 20:
                arduino.sendData([1])
                print("Warning")
                print(lidarData.Distance_i[1])

            else:
                arduino.sendData([0])
            tmpString = ""
            loopFlag = False
        else:
            tmpString += b.hex()+" "
        
        flag2c = False
    
    i +=1

ser.close()