
# -- coding: utf-8 --
# 普通2d视频，每帧按水平、垂直划分成两幅图，保存.

import glob
import os
import cv2

def isFlat(_img):
    num = 0
    if _img.shape[2] == 3:
        img = cv2.cvtColor(_img, cv2.COLOR_BGR2GRAY)
    else:
        img = _img
    for i in range(1, img.shape[0] - 1):
        for j in range(1, img.shape[1] - 1):
            diff = img[i, j] * 4 - img[i - 1, j] - img[i + 1, j] - img[i, j - 1] - img[i, j + 1]
            if (abs(diff) / 4 > 5):
                num += 1
    ratio = float(num) / ((img.shape[0] - 2) * (img.shape[1] - 2))
    if ratio > 0.02:
        return False
    else:
        return True


videoDir = r'D:\data\vr\h3d\h3d_1'
videoList = 'list.txt'
imageDir1 = 'image1'
imageDir2 = 'image2'
interval = 10
width = 48
height = 32

imageFullDir1 = os.path.join(videoDir, imageDir1)
if os.path.exists(imageFullDir1) == False:
    os.makedirs(imageFullDir1)
imageFullDir2 = os.path.join(videoDir, imageDir2)
if os.path.exists(imageFullDir2) == False:
    os.makedirs(imageFullDir2)

with open(os.path.join(videoDir, videoList), 'r') as f:
    names = f.readlines()

#print (glob.glob(os.path.join(videoDir, '*')))
#for file in (glob.glob(videoDir)):

#for file in (os.listdir(videoDir)):

horiz = True
for file in names:
    file = file.strip().split(' ')
    file = os.path.join(videoDir, file[0])
    cap = cv2.VideoCapture(file)

    # Check if camera opened successfully
    if (cap.isOpened() == False):
        print("Error opening %s" % file)
        continue
    print (file)
    frameNum = 0
    while (cap.isOpened()):
        # Capture frame-by-frame
        ret, frame = cap.read()
        if not (frameNum % interval == 0):
            frameNum += 1
            continue
        if frame is None:
            break
        if ret == True:
            imageName1 = "%s\\%s_%d_1.jpg" % (imageFullDir1, os.path.basename(file), frameNum)
            imageName2 = "%s\\%s_%d_2.jpg" % (imageFullDir2, os.path.basename(file), frameNum)
            print ('saving ' + imageName1)
            if horiz:
                smallImg1 = cv2.resize(frame[0:frame.shape[0], 0:frame.shape[1] / 2], (width, height))
                smallImg2 = cv2.resize(frame[0:frame.shape[0], frame.shape[1] / 2 : frame.shape[1]], (width, height))
            else:
                smallImg1 = cv2.resize(frame[0:frame.shape[0] / 2, 0:frame.shape[1]], (width, height))
                smallImg2 = cv2.resize(frame[frame.shape[0] / 2 :frame.shape[0], 0 : frame.shape[1]], (width, height))

            if isFlat(smallImg1) and isFlat(smallImg2):
                continue
            cv2.imwrite(imageName1, smallImg1)
            cv2.imwrite(imageName2, smallImg2)
            frameNum += 1
            horiz = not horiz


