## pixelLoader.py
read target picture and save all pixel info into the appointed directory
```
python pixelLoader.py [targetPic] [dest]
```
Output for this script is a file containing pixels' rgb value matrix and size information. For example a image file with size 2*2, the output file is like:

- 2 2
- 0 0 0 5 5 5
- 2 2 2 1 1 1

## testCaseGenerator.py
Generate data that simulates picture information with resolution from 256*256 to 8192*8192
```
python testCaseGenerator.py
```

## Serial_InterFrame_Difference.cpp
return a txt file containing difference matrix after processing
Remember to set C++ version to 11 or higher when compiling
```
g++  Serial_InterFrame_Difference.cpp -std=c++11 -o out
```

## InterFrame_Difference_OpenACC.cpp
Perform parallel code using openACC and test on Shared Computer Cluster with a tesla GPU card. There're some syntax in split function which is not supported by old version C++, so do things as showed below
```
pgc++ -Minfo=accel -fast -acc -ta=tesla:cc60 InterFrame_Difference_OpenACC.cpp -o out_acc -std=c++11
```
This version of parallel code is modified for ease of doing benchmark on test cases, so you might need to do 
```
./out_acc [128,256...]
```
which will load files with name test128_0.txt, test128_1.txt...

May need to modify some variables for running on other cases except cases generated by the script.

## Sharpening codes
To run the OpenACC version of the code, please follow the readme.txt on SCC.

To run the serial code on local computer, please change the path of the test file to your own directory.

For the 1d version code:
```
g++ sharpening1d.cpp -std=c++11 -o 1d
```

To run the code you need to change the argument, for example:

```
./1d 256
```
For 2d and 3d version, you can compile and run the code directly without argv.
