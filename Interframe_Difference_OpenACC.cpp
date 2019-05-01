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
#include <iterator>
#include <chrono>
using namespace std;


vector<string> Split(const string& subject)
{
    istringstream ss{subject};
    using StrIt = istream_iterator<string>;
    vector<string> container{StrIt(ss), StrIt()};
    return container;
}


int* loadPixel(string targetFile, int i, int j){
    
    
    int* restrict const pixels = (int*) malloc(i*j*3*sizeof(int));

    ifstream infile(targetFile);
    string curline;
    getline(infile,curline);
    
    // Load pixels into pixel array
    for(int a=0;a<i;a++){
        getline(infile,curline);
        vector<string> curRes = Split(curline);
        for(int b=0;b<j;b++){
            for(int rgb=0;rgb<3;rgb++){
                pixels[a*j+b*3+rgb] = stoi(curRes[b*3+rgb]);
            }
        }
    }
    
    return pixels;
}


int main(int argc, const char * argv[]) {
    auto IOstart = chrono::steady_clock::now();
#pragma acc init
    const string target = argv[1];
    string targetFile = "./tests/test"+target+"_0.txt";
    string targetFile1 = "./tests/test"+target+"_1.txt";
    
    const int i = stoi(argv[1]);
    const int j = stoi(argv[1]);
    
    ofstream outFile;
    outFile.open("./result_acc/result_"+target+".txt");
    
    const auto pixel = loadPixel(targetFile, i, j);
    const auto pixel2 = loadPixel(targetFile1, i, j);
    
    
    //const int out[i][j][3];
    
    int* restrict const out = (int*) malloc(i*j*3*sizeof(int));
    auto start1 = chrono::steady_clock::now();
#pragma acc data copy(pixel[0:i*j*3]) copy(pixel2[0:i*j*3]) create(out[0:i*j*3])
    {
    auto start = chrono::steady_clock::now();
#pragma acc kernels
        {
    #pragma acc loop independent
            for(int a=0;a<i;a++){
                for(int b=0;b<j;b++){
                    for(int rgb=0;rgb<3;rgb++){
                        out[a*j*3+b*3+rgb] = abs(pixel[a*j*3+b*3+rgb] - pixel2[a*j*3+b*3+rgb]);
                    }
                }
            }
        }
    
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
        auto elapsed2 = chrono::duration_cast<chrono::microseconds>(end - start1);
        auto copy = chrono::duration_cast<chrono::microseconds>(start - start1);
    
        cout << "Without copy: " << elapsed.count() << " microseconds." << endl;
        cout << "With copy: " << elapsed2.count() << " microseconds." << endl;
        cout << "Copy: " << copy.count() << " microseconds." << endl;
    }
    
    for(int a=0;a<i;a++){
        for(int b=0;b<j;b++){
            for(int rgb=0;rgb<3;rgb++){
                outFile << out[a*j*3+b*3+rgb] << " ";
            }
        }
        outFile << endl;
    }
    
    outFile.close();
    
    auto IOend = chrono::steady_clock::now();
    auto IO = chrono::duration_cast<chrono::microseconds>(IOend - IOstart);
    cout << "With I/O: "<< IO.count() <<" microseconds." << endl;
    
    return 0;
}
