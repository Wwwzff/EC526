//
//  main.cpp
//  FinalProject
//
//  Created by zifan wang on 4/4/19.
//  Copyright © 2019 zifan wang. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


vector<string> Split(const string& subject)
{
    istringstream ss{subject};
    using StrIt = istream_iterator<string>;
    vector<string> container{StrIt{ss}, StrIt{}};
    return container;
}

vector<int> getSize(string targetFile){
    ifstream infile(targetFile);
    
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

int*** loadPixel(string targetFile){
    
    int*** pixels;
    auto size = getSize(targetFile);
    int i = size[0];
    int j = size[1];
    
    ifstream infile(targetFile);
    string curline;
    getline(infile,curline);
    
    pixels = new int **[i];
    // Load pixels into pixel array
    for(int a=0;a<i;a++){
        pixels[a] = new int *[j];
        getline(infile,curline);
        vector<string> curRes = Split(curline);
        for(int b=0;b<j;b++){
            pixels[a][b] = new int [3];
            for(int rgb=0;rgb<3;rgb++){
                pixels[a][b][rgb] = stoi(curRes[b*3+rgb]);
            }
        }
    }
    
    return pixels;
}


int main(int argc, const char * argv[]) {
    
    string targetFile = "./out.txt";
    string targetFile1 = "./out1.txt";
    
    ofstream outFile;
    outFile.open("./result.txt");
    
    auto pixel = loadPixel(targetFile);
    auto pixel2 = loadPixel(targetFile1);
    auto sizes = getSize(targetFile);
    
    int i = sizes[0];
    int j = sizes[1];
    
    cout << sizes[0] << " " << sizes[1] << endl;
    cout << pixel[0][0][0] << endl;
    
    int*** outPixels = new int ** [i];
    
    for(int a=0;a<i;a++){
        for(int b=0;b<j;b++){
            outPixels[a] = new int * [j];
            for(int rgb=0;rgb<3;rgb++){
                outPixels[a][b] = new int [3];
                outPixels[a][b][rgb] = pixel2[a][b][rgb] - pixel[a][b][rgb];
                outFile << outPixels[a][b][rgb] << " ";
            }
        }
        outFile << endl;
    }
    
    outFile.close();
    return 0;
}
