from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import sys


def loadPixel(picDir, outPath):
    img = Image.open(picDir)
    imgsize = img.size
    img_matrix = np.array(img)

    outFile = open(outPath, "wb")

    img_matrix.tostring()
    outFile.write(img_matrix)
    outFile.close()


if __name__ == "__main__":
    dest = sys.argv[2]
    target = sys.argv[1]
    loadPixel(target, dest)
