#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>

using namespace std;

vector<string> Split(const string& subject)
{
    istringstream ss{subject};
    using StrIt = istream_iterator<string>;
    vector<string> container{StrIt{ss}, StrIt{}};
    return container;
}


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
        i = stoi(is);
        j = stoi(js);
        outsize.push_back(i);
        outsize.push_back(j);
    }

    return outsize;

}


int kernel(int*** pixel, int a, int b, int rgb){
    vector<int> kernel_list;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            kernel_list.push_back(pixel[a + i][b + j][rgb]);
        }
    }
    sort(kernel_list.begin(), kernel_list.end());
    int output = kernel_list[4];
    return output;
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


    for(int a=1;a<i + 1;a++){
        getline(infile,curline);
        vector<string> curRes = Split(curline);
        for(int b=1;b<j+1;b++){
            for(int rgb=0;rgb<3;rgb++){
                pixels[a][b][rgb] = stoi(curRes[(b-1)*3+rgb]);
            }
        }
    }
    for(int a=1; a<i+1; a++){
        for(int rgb=0;rgb<3;rgb++){
            pixels[a][0][rgb] = pixels[a][1][rgb];
            pixels[a][j+1][rgb] = pixels[a][j][rgb];
        }
    }
    for(int b=1; b<j+1; b++){
        for(int rgb=0;rgb<3;rgb++){
            pixels[0][b][rgb] = pixels[1][b][rgb];
            pixels[i+1][b][rgb] = pixels[i][b][rgb];
        }
    }
    for(int rgb=0;rgb<3;rgb++){
        pixels[0][0][rgb] = pixels[1][1][rgb];
        pixels[i+1][j+1][rgb] = pixels[i][j][rgb];
        pixels[i+1][0][rgb] = pixels[i][1][rgb];
        pixels[0][j+1][rgb] = pixels[1][j][rgb];
    }
    return pixels;

}


int main(int argc, const char * argv[]) {
    string targetFile = "./out.txt";

    ofstream outFile;
    outFile.open("./result.txt");
    auto start = chrono::steady_clock::now();
    int*** pixel = loadPixel(targetFile);
    auto sizes = getSize(targetFile);

    int j = sizes[0];
    int i = sizes[1];

//    cout << sizes[0] << " " << sizes[1] << endl;
//
//    cout << pixel[0][0][0] << endl;

    int*** outPixels = new int ** [i + 2];
    for(int a=0;a<i + 2;a++){
        outPixels[a] = new int * [j + 2];
        for(int b=0;b<j + 2;b++){
            outPixels[a][b] = new int [3];
        }
    }

    for(int a=1;a<i + 1;a++){
        for(int b=1;b<j + 1;b++){
            for(int rgb=0;rgb<3;rgb++) {
                outPixels[a][b][rgb] = 255 - kernel(pixel, a, b, rgb);
                outFile << outPixels[a][b][rgb] << " ";
            }
        }
        outFile << endl;
    }
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "It took me " << elapsed.count() << " microseconds." << endl;
    outFile.close();
    return 0;
}
