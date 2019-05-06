#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>

using namespace std;

#define i 256
#define j 256

int* loadPixel(string targetFile){

    int* pixels = (int*) malloc((i+2)*(j+2)*3*sizeof(int));

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
                pixels[a*j+b*3+rgb] = stoi(curRes[(b-1)*3+rgb]);
            }
        }
    }

    
        for(int a=1; a<i+1; a++){
        	for(int rgb=0;rgb<3;rgb++){
            pixels[a*j+0*3+rgb] = pixels[a*j+1*3+rgb];
            pixels[a*j+(j+1)*3+rgb] = pixels[a*j+j*3+rgb];
        }
    }
        for(int b=1; b<j+1; b++){
        	for(int rgb=0;rgb<3;rgb++){
            pixels[0*j+b*3+rgb] = pixels[1*j+b*3+rgb];
            pixels[(i+1)*j+b*3+rgb] = pixels[i*j+b*3+rgb];
        }
    }
    for(int rgb=0;rgb<3;rgb++){
        pixels[0*j+0*3+rgb] = pixels[1*j+1*3+rgb];
        pixels[(i+1)*j+(j+1)*3+rgb] = pixels[i*j+j*3+rgb];
        pixels[(i+1)*j+0*3+rgb] = pixels[i*j+1*3+rgb];
        pixels[0*j+(j+1)*3+rgb] = pixels[1*j+j*3+rgb];
    }

    return pixels;

}


int main(int argc, const char * argv[]) {
#pragma acc init
    string targetFile = "./out.txt";
    ofstream outFile;
    outFile.open("./result.txt");

    int* restrict const out = (int*) malloc((i+2)*(j+2)*3*sizeof(int));


    auto start = chrono::steady_clock::now();

    const auto pixel = loadPixel(targetFile);


#pragma acc data copyin(pixel[0:(i+2)*(j+2)*3]) copy(out[0:(i+2)*(j+2)*3])

// #pragma acc data create(out[0:(i+2)*(j+2)*3])

#pragma acc kernels
        {
    #pragma acc loop independent
    for(int a=1;a<i + 1;a++){
        for(int b=1;b<j + 1;b++){
            for(int rgb=0;rgb<3;rgb++) {
            out[a*j*3+b*3+rgb] = 255 - (pixel[(a+1)*j+(b-1)*3+rgb]+pixel[(a+1)*j+b*3+rgb]+pixel[(a+1)*j+(b+1)*3+rgb]+pixel[a*j+(b-1)*3+rgb]+pixel[a*j+b*3+rgb]+pixel[a*j+(b+1)*3+rgb]+pixel[(a-1)*j+(b-1)*3+rgb]+pixel[(a-1)*j+b*3+rgb]+pixel[(a-1)*j+(b+1)*3+rgb])/9;
            }
        }
    }
}

//    for(int a=1;a<i + 1;a++){
//        for(int b=1;b<j + 1;b++){
//            for(int rgb=0;rgb<3;rgb++) {
//                temp[a*j*3+b*3+rgb] = kernel(pixel, a, b, rgb);
//        }
//    }
//}

//#pragma acc parallel loop independent
//    for(int a=1;a<i + 1;a++){
//        for(int b=1;b<j + 1;b++){
//            for(int rgb=0;rgb<3;rgb++) {
//                out[a*j*3+b*3+rgb] = 255 - temp[a*j*3+b*3+rgb];
//            }
//        }
//    }



    for(int a=1;a<i + 1;a++){
        for(int b=1;b<j + 1;b++){
            for(int rgb=0;rgb<3;rgb++) {
                outFile << out[a*j*3+b*3+rgb] << " ";
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
