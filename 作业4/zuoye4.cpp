#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ceres/ceres.h>

using namespace std;
using namespace ceres;

struct curve_fitting_cost{
    curve_fitting_cost(double dx, double dy):x(dx), y(dy){}
    template <typename T>
    bool operator()(const T* const c, T* residual) const{
        residual[0] = T(y) - c[0]*cos(c[1]*(x + c[2])) + c[3]*x + c[4];
        return true;
    }

    const double x,y;
};

int main(){
    double c[5] = {0.416, 1.884, 1, 1.305, 1};
    cout << c[0] << endl;
    vector<double> data_x, data_y;
    double d;
    int i = 1;
    ifstream zz;
    zz.open("/home/milo/Projects/作业4/curve_fitting.txt");
    if(zz.is_open()) cout << "打开" << endl;
    while(zz >> d){
        if(i%2 == 1) {data_x.push_back(d/1000); i = 2;}
        else {data_y.push_back(d); i = 1;}
        }
    zz.close();

    cout << data_x.size() << "  " << data_y.size() << endl;
    Problem problem;
    for( int i = 0; i< data_x.size(); i++){
        problem.AddResidualBlock(
        new AutoDiffCostFunction<curve_fitting_cost, 1, 5>(
            new curve_fitting_cost(data_x[i], data_y[i])),
            nullptr,
            c
        );

        //cout << data_x[i] << "    " << data_y[i] << endl;
    }

    Solver::Options options;
    options.linear_solver_type = DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);
    cout << summary.BriefReport() << endl;
    cout << "a = " << c[0] << endl;
    cout << "b = " << c[1] << endl;
    cout << "t0 = " << c[2] << endl;
    cout << "c = " << c[3] << endl;
    cout << "d = " << c[4] << endl;
    
    return 0;
    }
