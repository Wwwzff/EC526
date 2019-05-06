#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;


vector<int> getSize(string targetfile){
    ifstream infile(targetfile);
    vector<int> outsize;
    if(!infile.is_open()){
        cout << "File not found!" << endl;
    }
    else{
        // File Opened
        // Load size info
        string is,js;
        int i,j;
        string size;
        getline(infile,size);
        istringstream iss(size);
        iss >> is >> js;
        // Convert string to int
        i = stoi(is);
        j = stoi(js);
        outsize.push_back(i);
        outsize.push_back(j);
    }

    return outsize;

}


int filter(int*** pixel, int a, int b, int rgb){
    int res = 0;
    int laplace[9] = {
             0,-1, 0,
            -1, 5,-1,
             0,-1, 0
//            0,0,0,
//            0,2,0,
//            0,0,-1
    };
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            res += laplace[i+3*j] * pixel[a+i-1][b+j-1][rgb];
        }
    }
    return res;
}


int*** loadPixel(string targetFile){
    auto size = getSize(targetFile);
    int j = size[0];
    int i = size[1];
    int*** pixels = new int **[i + 2];
    for(int a=0;a<i + 2;a++){
        pixels[a] = new int * [j + 2];
        for(int b=0;b<j+2;b++){
            pixels[a][b] = new int [3];
        }
    }


    ifstream infile(targetFile);
    string curline;
    getline(infile,curline);


    for(int a=1; a<i+1; a++){
        getline(infile,curline);
        istringstream ss{curline};
        using StrIt = istream_iterator<string>;
        vector<string> curRes{StrIt{ss}, StrIt{}};
        for(int b=1;b<j+1;b++){
            for(int rgb=0;rgb<3;rgb++){
                pixels[a][b][rgb] = stoi(curRes[(b-1)*3+rgb]);
            }
        }
    }


    for(int rgb=0;rgb<3;rgb++){
        for(int a=1; a<i+1; a++){
            pixels[a][0][rgb] = pixels[a][1][rgb];
            pixels[a][j+1][rgb] = pixels[a][j][rgb];
        }

        for(int b=1; b<j+1; b++){
            pixels[0][b][rgb] = pixels[1][b][rgb];
            pixels[i+1][b][rgb] = pixels[i][b][rgb];
        }

        pixels[0][0][rgb] = pixels[1][1][rgb];
        pixels[i+1][j+1][rgb] = pixels[i][j][rgb];
        pixels[i+1][0][rgb] = pixels[i][1][rgb];
        pixels[0][j+1][rgb] = pixels[1][j][rgb];
    }

    return pixels;

}


int main(int argc, const char * argv[]) {
    string targetFile = R"(C:\Users\zhoub\OneDrive\Desktop\test.txt)";

    ofstream outFile;
    outFile.open(R"(C:\Users\zhoub\OneDrive\Desktop\result.txt)");

    //return the size of the image: length,width
    auto sizes = getSize(targetFile);

    int*** pixel = loadPixel(targetFile);


    int j = sizes[0];
    int i = sizes[1];

    cout << sizes[0] << " " << sizes[1] << endl;

    cout << pixel[0][0][0] << endl;

    int*** outPixels = new int ** [i + 2];
    for(int a=0; a<i + 2; a++){
        outPixels[a] = new int * [j + 2];
        for(int b=0;b<j + 2;b++){
            outPixels[a][b] = new int [3];
        }
    }

    for(int a=1;a<i + 1;a++){
        for(int b=1;b<j + 1;b++){
            for(int rgb=0;rgb<3;rgb++) {
                outPixels[a][b][rgb] = 255 - filter(pixel, a, b, rgb);
                outFile << outPixels[a][b][rgb] << " ";
            }
        }
        outFile << endl;
    }
    outFile.close();
    return 0;
}
