"""
   *Face Tracking System Using Arduino - Python Code*
    Close the Arduino IDE before running this code to avoid Serial conflicts.
    Replace 'COM5' with the name of port where you arduino is connected.
    To find the port check Arduino IDE >> Tools >> port.
    Upload the Arduino code before executing this code.

    # Code by Harsh Dethe, 09 Sep 2018 #
"""

import cv2

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

cap = cv2.VideoCapture(0)


while 1:
    
    ret, img = cap.read()
      # cv2.resizeWindow('img', 500,500)
      #  cv2.line(img,(500,250),(0,250),(0,255,0),1)
      #  cv2.line(img,(250,0),(250,500),(0,255,0),1)
      # cv2.circle(img, (250, 250), 5, (255, 255, 255), -1)
       # gray  = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(img,1.024, minSize=(119, 119))
        
    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),5)

    cv2.imshow('img',img)
       
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break