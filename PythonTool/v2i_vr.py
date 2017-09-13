import glob
import os
import cv2

videoDir = r'D:\data\vr\video\h3d'
imageDir1 = 'image1'
imageDir2 = 'image2'
interval = 10

imageFullDir1 = os.path.join(videoDir, imageDir1)
if os.path.exists(imageFullDir1) == False:
    os.makedirs(imageFullDir1)
imageFullDir2 = os.path.join(videoDir, imageDir2)
if os.path.exists(imageFullDir2) == False:
    os.makedirs(imageFullDir2)
#print (glob.glob(os.path.join(videoDir, '*')))
#for file in (glob.glob(videoDir)):
for file in (os.listdir(videoDir)):
    file = os.path.join(videoDir, file)
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
            cv2.imwrite(imageName1, frame[0:frame.shape[0], 0:frame.shape[1] / 2])
            cv2.imwrite(imageName2, frame[0:frame.shape[0], frame.shape[1] / 2 : frame.shape[1]])
            frameNum += 1

