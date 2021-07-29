#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>
#include <Mmsystem.h>
#include <filesystem>
#include <thread>

#pragma comment(lib,"winmm.lib")

using namespace cv;
using namespace std;

char ascii_char[70] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
float unit = 3.7246;

ofstream file;
string save;

void terminalset() {

    SetConsoleTitleA("Bad_Apple");
    system("mode con cols=352 lines=122");
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = { 1, 0 };
    SetConsoleCursorInfo(hout, &info);

}

void txtwrite();
void play() {
    
    VideoCapture cap("Resources\\Bad_Apple.mp4");
    Mat image, resizeImage, gray;
    Mat detection_image = image.clone();

    if (!cap.isOpened()) {

        MessageBoxW(NULL, L"Load Video Error", L"Bad Apple in termial Error", MB_ICONERROR);
        return;
    }

    try {
        while (true) {
            
            string txt = "";
            cap.read(detection_image);
            cvtColor(detection_image, gray, COLOR_BGR2GRAY);
            resize(gray, resizeImage, Size(350,120));  //192,54(16:9), 150,100 || 180,100(9:16) , 200,100(1:1)

            int rowNumber = resizeImage.rows;
            int colNumber = resizeImage.cols;

            for (int i = 0; i < rowNumber; i++) {
                for (int j = 0; j < colNumber; j++) {
                    txt += ascii_char[int((resizeImage.at<uchar>(i, j)) / unit)];
                }
                txt += '\n';
            }
            const char* c = txt.c_str();
            printf_s("%s", c);

            imshow("Bad_Apple", detection_image);
            waitKey(10);
            system("cls");

            save = txt;
            thread t1(txtwrite);
            t1.detach();
        }
    }
    catch (...) {
        return;
    }
}

void txtwrite() {

    //PlaySound(TEXT("Resources\\Bad_Apple.wav"), NULL, SND_FILENAME);

    file.open("Resources\\bad_apple.txt");
    file << save << endl;
    file.close();
}

int main(void) {

    //Sleep(10000);
    terminalset();
    play();

    return 0;
}