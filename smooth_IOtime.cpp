#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>

using namespace std;

//vector<string> Split(const string& subject)
//{
//    istringstream ss{subject};
//    using StrIt = istream_iterator<string>;
//    vector<string> container{StrIt{ss}, StrIt{}};
//    return container;
//}

//int kernel(int* pixel, int a, int b, int rgb){
//    int kernel_list[9];
//    for(int k=0; k<9; k++){
//    for(int m = -1; m < 2; m++){
//        for(int n = -1; n < 2; n++){
//            kernel_list[k] = pixel[(a+m)*j+(b+n)*3+rgb];
//        }
//    }
//    }
//    sort(kernel_list, kernel_list+9);
//    int output = kernel_list[4];
//    return output;
//}

//int* loadPixel(string targetFile){
//
//    int* restrict const pixels = (int*) malloc((i+2)*(j+2)*3*sizeof(int));
//
//    ifstream infile(targetFile);
//    string curline;
//    getline(infile,curline);
//
//    for(int a=1;a<i + 1;a++){
//        getline(infile,curline);
//        istringstream ss{curline};
//        using StrIt = istream_iterator<string>;
//        vector<string> curRes{StrIt{ss}, StrIt{}};
////        vector<string> curRes = Split(curline);
//        for(int b=1;b<j+1;b++){
//            for(int rgb=0;rgb<3;rgb++){
//                pixels[a*j+b*3+rgb] = stoi(curRes[(b-1)*3+rgb]);
//            }
//        }
//    }
//
//    for(int a=1; a<i+1; a++){
//        for(int rgb=0;rgb<3;rgb++){
//            pixels[a*j+rgb] = pixels[a*j+3+rgb];
//            pixels[a*j+(j+1)*3+rgb] = pixels[a*j+j*3+rgb];
//        }
//    }
//
//    for(int b=1; b<j+1; b++){
//        for(int rgb=0;rgb<3;rgb++){
//            pixels[b*3+rgb] = pixels[j+b*3+rgb];
//            pixels[(i+1)*j+b*3+rgb] = pixels[i*j+b*3+rgb];
//        }
//    }
//
//    for(int rgb=0;rgb<3;rgb++){
//        pixels[rgb] = pixels[j+3+rgb];
//        pixels[(i+1)*j+(j+1)*3+rgb] = pixels[i*j+j*3+rgb];
//        pixels[(i+1)*j+rgb] = pixels[i*j+3+rgb];
//        pixels[(j+1)*3+rgb] = pixels[j+j*3+rgb];
//    }
//
//    return pixels;
//
//}
int* loadPixel(string targetFile, int i, int j){

    int* pixels = new int[(i+2)*(j+2)*3];

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
//#pragma acc parallel loop
    for(int rgb=0;rgb<3;rgb++){
        for(int a=1; a<i+1; a++){
            pixels[a*j+0*3+rgb] = pixels[a*j+1*3+rgb];
            pixels[a*j+(j+1)*3+rgb] = pixels[a*j+j*3+rgb];
        }

        for(int b=1; b<j+1; b++){
            pixels[0*j+b*3+rgb] = pixels[1*j+b*3+rgb];
            pixels[(i+1)*j+b*3+rgb] = pixels[i*j+b*3+rgb];
        }

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

    const int i = stoi(argv[1]);
    const int j = stoi(argv[2]);

    int* out = new int[(i+2)*(j+2)*3];


    auto start = chrono::steady_clock::now();

    const auto pixel = loadPixel(targetFile, i, j);

    for(int a=1;a<i + 1;a++){
        for(int b=1;b<j + 1;b++){
            for(int rgb=0;rgb<3;rgb++) {
                outFile << pixel[a*j*3+b*3+rgb] << " ";
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
