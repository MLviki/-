#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>
#include <math.h>
#include <vector>

#define PI acos(-1)

using namespace std;
using namespace cv;

float* Rotation(int xyz, int z){
    float s = z*PI/180;
    if(xyz == 1)
        {static float k[9] = {1,0,0,0,cos(s),-sin(s),0,sin(s),cos(s)}; return k;}
    if(xyz == 2)
        {static float k[9] = {cos(s),0,sin(s),0,1,0,-sin(s),0,cos(s)}; return k;}
    if(xyz == 3)
        {static float k[9] = {cos(s),-sin(s),0,sin(s),cos(s),0,0,0,1}; return k;}    
}

int main(){
    //float *m;
    //m = Rotation(3,30);
    float k1[3] = {1, 2, 0};
    Eigen::Matrix<double, 3, 1> m_p ;
    m_p << 1, 2, 0;
    
    cout << "原坐标: \n" << m_p << endl;
    Mat p = Mat(3, 1, CV_32FC1, k1);
    Mat Rot_z = Mat(3, 3, CV_32FC1, Rotation(3, 30));
    Mat result_op = Rot_z*p;    
    Eigen::AngleAxisd r_z_v(PI/6,Eigen::Vector3d(0, 0, 1));
    Eigen::Matrix3d r_z = r_z_v.matrix();
    Eigen::Matrix<double, 3, 1> result_ei = r_z * m_p;
    cout << "\n绕ZB轴旋转30度: " << endl;
    cout << "OpenCV 结果: \n" << result_op << endl;
    cout << "Eigen 结果: \n" << result_ei << endl << endl;
    
    float k2[3] = {2, -3, 1};
    Mat pingyi_op = Mat(3, 1, CV_32FC1, k2);
    result_op += pingyi_op;
    Eigen::Matrix<double, 3, 1> pingyi_ei;
    pingyi_ei << 2, -3, 1;
    result_ei += pingyi_ei;
    cout << "平移后: " << endl;
    cout << "OpenCV 结果: \n" << result_op << endl;
    cout << "Eigen 结果: \n" << result_ei << endl << endl;

    Mat Rot_y = Mat(3, 3, CV_32FC1, Rotation(2, 45));
    result_op = Rot_z * Rot_y * p;
    Eigen::AngleAxisd r_y_v(PI/4, Eigen::Vector3d(0, 1, 0));
    Eigen::Matrix3d r_y = r_y_v.matrix();
    result_ei = r_z * r_y * m_p;
    cout << "再绕y轴45度后: " << endl;
    cout << "OpenCV 结果: \n" << result_op << endl;
    cout << "Eigen 结果: \n" << result_ei << endl << endl;

    Mat Rot_x = Mat(3, 3, CV_32FC1, Rotation(1, 60));
    result_op = Rot_z * Rot_y * Rot_x * p;
    Eigen::AngleAxisd r_x_v(PI/3, Eigen::Vector3d(1, 0, 0));
    Eigen::Matrix3d r_x = r_x_v.matrix();
    result_ei = r_z * r_y * r_x * m_p;
    cout << "再绕x轴60度后: " << endl;
    cout << "OpenCV 结果: \n" << result_op << endl;
    cout << "Eigen 结果: \n" << result_ei << endl;

    return 0;


    
    
    //cout << m_p << endl << Rot_z << endl;
    
    return 0;
}
