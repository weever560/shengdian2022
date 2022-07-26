# Untitled - By:阿布  - 周日 7月 19 2021

import sensor, image, time

from pid import PID
from pyb import Servo
from pyb import UART,LED
import json
import ustruct
LED(1).on()
LED(2).on()
LED(3).on()

roi = (0, 0, 200, 80)


red_threshold  = (36, 54, 20, 75, -17, 72)


sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.3
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
clock = time.clock() # Tracks FPS.

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

def sending_data(cx,cy,cw,ch,rog):
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhhhb",      #格式为俩个字符俩个短整型(2字节)
                   0x2C,                      #帧头1
                   0x12,                      #帧头2
                   int(cx), # up sample by 4   #数据1
                   int(cy), # up sample by 4    #数据2
                   int(cw), # up sample by 4    #数据1
                   int(ch), # up sample by 4    #数据2
                   int(rog),
                   0x5B)
    uart.write(data);   #必须要传入一个字节数组

while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # Take a picture and return the image.
    img.draw_rectangle(roi, color=(255,0,0))#绘制出roi区域
    blobs = img.find_blobs([red_threshold],roi=roi)
    if blobs:
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob.rect()) # rect
        img.draw_cross(max_blob.cx(), max_blob.cy()) # cx, cy


        cx=max_blob[5]
        cy=max_blob[6]
        cw=max_blob[2]
        ch=max_blob[3]
        rog=1
        FH = bytearray([0x2C,0x12,cx,cy,cw,ch,rog,0x5B])
        uart.write(FH)
        print(cx,cy,cw,ch,rog)
    else:
        cx=0
        cy=0
        cw=0
        ch=0
        rog=0
        FH = bytearray([0x2C,0x12,cx,cy,cw,ch,rog,0x5B])
        uart.write(FH)
        print(cx,cy,cw,ch,rog)
