#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <sstream>

#define hConsole GetStdHandle(STD_OUTPUT_HANDLE)
#define ConsoleClear() SetConsoleCursorPosition(hConsole, { 0, 0 })
#define GetRGB(r, g, b, Pixel) ( \
    std::string("\x1b[38;2;") + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + Pixel \
)

#define HideConsoleCursor() { \
    CONSOLE_CURSOR_INFO cursorInfo; \
    GetConsoleCursorInfo(hConsole, &cursorInfo); \
    cursorInfo.bVisible = false; \
    SetConsoleCursorInfo(hConsole, &cursorInfo); \
}

std::pair<int, int> GetTerminalSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    return std::make_pair(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

class PixelPlayer {
public:
    void operator()(cv::VideoCapture cap, int delay) {
        cv::Mat detection_image, resizeImage;
        try {
            while (true) {
                bool ref = cap.read(detection_image);
                if (!ref) {
                    MessageBoxW(NULL, L"Playback completed", L"INFO", MB_ICONINFORMATION);
                    break;
                }
                int x = GetTerminalSize().first - 1;
                int y = GetTerminalSize().second - 1;
                resize(detection_image, resizeImage, cv::Size(x, y));

                int rowNumber = resizeImage.rows;
                int colNumber = resizeImage.cols;

                ConsoleClear();
                std::stringstream ss;
                for (int i = 0; i < rowNumber; i++) {
                    for (int j = 0; j < colNumber; j++) {
                        int r = resizeImage.at<cv::Vec3b>(i, j)[2];
                        int g = resizeImage.at<cv::Vec3b>(i, j)[1];
                        int b = resizeImage.at<cv::Vec3b>(i, j)[0];
                        ss << GetRGB(r, g, b, "█");
                    }
                    ss << std::endl;
                }
                printf_s("%s", ss.str().c_str());
                resizeImage.release();
                detection_image.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));

                if (_kbhit()) {
                    char key = _getch();
                    if (key == 27) {
                        break;
                    }
                }
            }
            resizeImage.release();
            detection_image.release();
            cv::destroyAllWindows();
        }
        catch (...) {
            return;
        }
    }
}PlayerP;

class ASCIIPlayer {
public:
    void operator() (cv::VideoCapture cap, int delay) {
        char ascii_char[70] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
        float unit = 3.723;
        cv::Mat detection_image, resizeImage, gray;
        try {
            while (true) {
                bool ref = cap.read(detection_image);
                if (!ref) {
                    MessageBoxW(NULL, L"Playback completed", L"INFO", MB_ICONINFORMATION);
                    break;
                }
                int x = GetTerminalSize().first - 1;
                int y = GetTerminalSize().second - 1;
                resize(detection_image, resizeImage, cv::Size(x, y));
                cvtColor(resizeImage, gray, cv::COLOR_BGR2GRAY);
                bitwise_not(gray, gray);

                int rowNumber = resizeImage.rows;
                int colNumber = resizeImage.cols;

                ConsoleClear();
                std::stringstream ss;
                for (int i = 0; i < rowNumber; i++) {
                    for (int j = 0; j < colNumber; j++) {
                        int r = resizeImage.at<cv::Vec3b>(i, j)[2];
                        int g = resizeImage.at<cv::Vec3b>(i, j)[1];
                        int b = resizeImage.at<cv::Vec3b>(i, j)[0];
                        ss << GetRGB(r, g, b, ascii_char[int(gray.at<uchar>(i, j) / unit)]);
                        //ss << ascii_char[int(gray.at<uchar>(i, j) / unit)];
                    }
                    ss << std::endl;
                }
                printf_s("%s", ss.str().c_str());
                gray.release();
                resizeImage.release();
                detection_image.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(delay / 30));

                if (_kbhit()) {
                    char key = _getch();
                    if (key == 27) {
                        break;
                    }
                }
            }
            gray.release();
            resizeImage.release();
            detection_image.release();
            cv::destroyAllWindows();
        }
        catch (...) {
            return;
        }
    }
}PlayerA;

int main(int argc, char* argv[]) {
    HideConsoleCursor();
    std::string path = "Resources\\Blue Archive.mp4";
    //if (argc > 1) {
    //    path = argv[1];
    //}
    //else {
    //    printf_s("Using：fileName.exe <filePath>\n");
    //    printf_s("Ex：fileName.exe \"panda.ma4\"\n");
    //    return 1;
    //}
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) {
        MessageBoxW(NULL, L"Error: Failed to load video", L"INFO", MB_ICONERROR);
        return 1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = static_cast<int>(1000.0 / fps);
    PlayerA(cap, delay);
    return 0;
}
