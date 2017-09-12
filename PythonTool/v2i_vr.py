import glob
import os
import cv2

videoDir = 'd:\\data\\vr\\*'
imageDir = 'image'

#print (glob.glob(os.path.join(videoDir, '*')))
for file in (glob.glob(videoDir)):
    cap = cv2.VideoCapture(file)

    # Check if camera opened successfully
    if (cap.isOpened() == False):
        print("Error opening video stream or file")
        continue

    print (file)
    frameNum = 0
    while (cap.isOpened()):
        # Capture frame-by-frame
        ret, frame = cap.read()
        if ret == True:
            imageName = "%s\\%s\\%s_%d.jpg" % (os.path.dirname(file), imageDir, os.path.basename(file), frameNum)
            print ('saving ' + imageName)
            cv2.imwrite(imageName, frame[0:frame.shape[0], 0:frame.shape[1] / 2])
            frameNum += 1

