// orange2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{

    //需要用绝对路径修改图片路径
    
    Mat img = imread("/home/milo/Projects/作业3/orange.jpg"), dst, hsv;
    Mat draw;
    img.copyTo(draw);
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;

    dst = img.clone();
    //cvtColor(img, hsv, COLOR_BGR2GRAY);
    //Scalar color_min(16, 43, 46);
    //Scalar color_max(34, 255, 255);
    inRange(img, Scalar(0, 0, 46), Scalar(34, 255,255), dst);
    imshow("直方图", dst);
    //waitKey(0);

    findContours(dst, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));
    vector<vector<Point>> contours_ploy(contours.size()); //使描绘轮廓的向量与原得到的轮廓向量有相同的大小。
    vector<Rect> ploy_rect(contours.size());
    vector<Point2f> ccs(contours.size());
    vector<float> radius(contours.size());

    for (size_t i = 0; i < contours.size(); i++) {
        approxPolyDP(contours[i], contours_ploy[i], 3, true);
        minEnclosingCircle(contours_ploy[i], ccs[i], radius[i]);
    }

    
    for (size_t i = 0; i < contours.size(); i++) {
        if (radius[i] > 50) {
            Mat ROI = dst(Rect((int)ccs[i].x, (int)ccs[i].y, (int)radius[i]/2, (int)radius[i]/2));
            //dst = ROI.clone();
            //inRange(ROI, Scalar(0, 0, 46), Scalar(34, 255, 255), dst1);
            Mat_<uchar>::iterator itor = ROI.begin<uchar>();
            Mat_<uchar>::iterator itorEnd = ROI.end<uchar>();
            int w_count = 0, b_count = 0;
            for (; itor != itorEnd; ++itor) {
                if ((*itor) > 0)
                    w_count++;
                else b_count++;
            }
            if (w_count - b_count > 500) {

           
                //for (size_t t = 0; t < contours.size(); t++) {
                    Scalar color = Scalar(0, 255, 0);
                    circle(draw, ccs[i], (int)radius[i], color, 2, 8);
                //}


            }
        }
    }
    imshow("检测", draw);
    waitKey(0);
    return 0;


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
