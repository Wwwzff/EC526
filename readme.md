## pixelLoader.py
read target picture and save all pixel info into the appointed directory
```
python pixelLoader.py [targetPic] [dest]
```
Output for this script is a file containing pixels' rgb value matrix and size information. For example a image file with size 2*2, the output file is like:

- 2 2
- 0 0 0 5 5 5
- 2 2 2 1 1 1

## Serial_InterFrame_Difference.cpp
return a txt file containing difference matrix after processing
Remember to set C++ version to 11 or higher when compiling
```
g++  Serial_InterFrame_Difference.cpp -std=c++11 -o out
```
