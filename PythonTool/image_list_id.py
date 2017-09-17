# -- coding: utf-8 --

# 给定根目录，生成子目录下的图像列表和标签，用于caffe图像分类训练.
import os
import random
def image_list_id(imageRoot):
    label = 0
    for filename in os.listdir(imageRoot):
        if os.path.isdir(os.path.join(imageRoot, filename)):
            for imagename in os.listdir(os.path.join(imageRoot, filename)):
                name, ext = os.path.splitext(imagename)
                ext = ext[1:]
                if ext == 'jpg' or ext == 'png' or ext == 'bmp':
                    with open('list.txt', 'a') as f:
                        f.write('%s %d\n' %((filename + '/' + imagename), label))
            label += 1

def shuffle_split(listFile, trainFile, valFile):
    with open(listFile, 'r') as f:
        records = f.readlines()
    random.shuffle(records)
    num = len(records)
    trainNum = int(num * 0.8)
    with open(trainFile, 'w') as f:
        f.writelines(records[0:trainNum])
    with open(valFile, 'w') as f1:
        f1.writelines(records[trainNum:])

#image_list_id(r'D:\data\vr\siamese')
shuffle_split('list.txt', 'train.txt', 'val.txt')
