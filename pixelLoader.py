from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import sys


def loadPixel(picDir, outPath):
    outFile = open(outPath, "w")

    img = Image.open(picDir)
    img_matrix = np.array(img)
    print(img_matrix.shape)


    outFile.write(str(a[1])+" "+str(a[0])+"\n")

    for i in range(img.size[1]):
        for j in range(img.size[0]):
            for rgb in range(3):
                outFile.write(str(img_matrix[i][j][rgb]))
                outFile.write(" ")
        outFile.write("\n")

    outFile.close()


def savePic(textDir, outPath):
    raw = np.loadtxt(textDir)
    img_matrix = np.reshape(raw, (raw.shape[0], int(raw.shape[1]/3), 3))
    plt.imsave(outPath, img_matrix)


if __name__ == "__main__":
    dest = sys.argv[2]
    target = sys.argv[1]
    #target = "./out.jpg"
    #dest = "./out1.txt"
    loadPixel(target, dest)
    savePic("./result.txt", "1.jpg")
