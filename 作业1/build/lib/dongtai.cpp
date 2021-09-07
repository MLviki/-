/*
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
*/
#include "dongtai.h"

using namespace std;

int main(){
  float k[6] = {1,4,2,5,3,6};
  cv::Mat M1 = cv::Mat::ones(2, 3, CV_32FC1);
  cv::Mat M2 = cv::Mat(3, 2, CV_32FC1, k);
  cv::Mat M3 = M1*M2 ;
  cout << "M1 = \n" << M1 << endl ;
  cout << "M2 = \n" << M2 << endl ;
  cout << "This is the result of opencv: \n" << M3 << endl;
  

  Eigen::Matrix<int, 2, 3> matrix_23 ;
  matrix_23 << 1, 1, 1, 1, 1, 1;
  Eigen::Matrix<int, 3, 2> matrix_32 ;
  matrix_32 << 1, 4, 2, 5, 3, 6 ;
  Eigen::Matrix<int, 2, 2> result_22 = matrix_23 * matrix_32;
  cout << "This is the result of Eigen: \n" << result_22 << endl;
  
  return 0;
  }
