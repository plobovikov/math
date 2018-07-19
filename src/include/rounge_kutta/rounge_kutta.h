#ifndef ROUNGE_KUTTA
#define ROUNGE_KUTTA
#include <iomanip>
#include <vector>

#include "matrix.h"

using namespace std;

template<int>
struct signature;
template<>
struct signature<2>{
    typedef std::function<double(double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2]);
    }
};
template<>
struct signature<3>{
    typedef std::function<double(double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3]);
    }
};
template<>
struct signature<4>{
    typedef std::function<double(double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4]);
    }
};
template<>
struct signature<5>{
    typedef std::function<double(double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5]);
    }
};
template<>
struct signature<6>{
    typedef std::function<double(double,double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5],v[6]);
    }
};
template<>
struct signature<7>{
    typedef std::function<double(double,double,double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7]);
    }
};
template<>
struct signature<8>{
    typedef std::function<double(double,double,double,double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8]);
    }
};
template<>
struct signature<9>{
    typedef std::function<double(double,double,double,double,double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]);
    }
};
template<>
struct signature<10>{
    typedef std::function<double(double,double,double,double,double,double,double,double,double,double,double)> function;
    static double compute(function f,std::vector<double>v){
        return f(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10]);
    }
};

template<int EquationsNum = 2>
class diffur_system_solver{
    double x0,x;
    double h;
    int count = 0;

    std::vector<typename signature<EquationsNum>::function>functions;
    std::vector<double>initial_conditions;
    matrix<double>result;

public:
    diffur_system_solver(double x0,double x,double h) : x0(x0),x(x),h(h),result(EquationsNum,(x-x0)/h + 1){

    }

    template<class ...Args>void set_functions(Args...);
    template<class T,class ...Args>
    void set_functions(T f,Args ...args){
        functions.push_back(f);
        set_functions(args...);
    }
    template<class T>
    void set_functions(T f){
        functions.push_back(f);
    }

    template<class ...Args>void set_conditions(Args...);
    template<class T,class ...Args>
    void set_conditions(T c,Args ...args){
        initial_conditions.push_back(c);
        set_conditions(args...);
    }
    template<class T>
    void set_conditions(T c){
         initial_conditions.push_back(c);
    }
    matrix<double> get(){return result;}
 
    bool operator()(){
        /*initial condition setup*/
        count = 0;
        for(int i = 0;i < EquationsNum;i++)
            result(i,0) = initial_conditions[i];

        /*Rounge Kutta method compute*/

        /*for each of the segments*/
        for(double _x = x0+h;_x <= x;_x += h)
        {
            std::vector<double>k1;
            std::vector<double>k2;
            std::vector<double>k3;
            std::vector<double>k4;
            std::vector<double>k5;

            compute_k1(_x,k1);
            compute_k2(_x,k2,k1);
            compute_k3(_x,k3,k1,k2);
            compute_k4(_x,k4,k1,k3);
            compute_k5(_x,k5,k1,k3,k4);

            /*step 5 compute functions value*/
            for(int i = 0;i < EquationsNum;i++){

                result(i,count+1) = result(i,count) + (1/2.0)*(k1[i]+4*k4[i]+k5[i]);
            }
            count++;
        }
        pr->setValue(x);
        return true;
    }
private:
    void compute_k1(double _x,std::vector<double>&k1){
        /*pack all params on this iteration in vector*/
        std::vector<double>params;
        params.push_back(_x);
        for(int j = 0;j < EquationsNum;j++)
            params.push_back(result(j,count));

        /*for each searching function*/

        /*step 1 compute k1i factors*/
        for(int i = 0;i < EquationsNum;i++){
            k1.push_back((h/3)*signature<EquationsNum>::compute(functions[i],params));
        }


    }
    void compute_k2(double _x,std::vector<double>&k2,std::vector<double>k1){
        std::vector<double>params;
        /*pack all params on this iteration in vector*/
        params.push_back(_x + h/3);
        for(int j = 0;j < EquationsNum;j++)
            params.push_back(result(j,count) + k1[j]);

        /*step 2 compute k2i factors*/
        for(int i = 0;i < EquationsNum;i++){
            k2.push_back((h/3)*signature<EquationsNum>::compute(functions[i],params));
        }

    }
    void compute_k3(double _x,std::vector<double>&k3,std::vector<double>k1,std::vector<double>k2){
        std::vector<double>params;
        /*pack all params on this iteration in vector*/
        params.push_back(_x + h/3);
        for(int j = 0;j < EquationsNum;j++)
            params.push_back(result(j,count) + k1[j]/2 + k2[j]/2);

        /*step 3 compute k3i factors*/
        for(int i = 0;i < EquationsNum;i++){
            k3.push_back((h/3)*signature<EquationsNum>::compute(functions[i],params));
        }

    }
    void compute_k4(double _x,std::vector<double>&k4,std::vector<double>k1,std::vector<double>k3){
        std::vector<double>params;
        /*pack all params on this iteration in vector*/
        params.push_back(_x + h/2);
        for(int j = 0;j < EquationsNum;j++)
            params.push_back(result(j,count) + (3/8.0)*k1[j] + (9/8.0)*k3[j]);

        /*step 4 compute k4i factors*/
        for(int i = 0;i < EquationsNum;i++){
            k4.push_back((h/3)*signature<EquationsNum>::compute(functions[i],params));
        }
    }
    void compute_k5(double _x,std::vector<double>&k5,std::vector<double>k1,std::vector<double>k3,std::vector<double>k4){
        std::vector<double>params;
        /*pack all params on this iteration in vector*/
        params.push_back(_x + h);
        for(int j = 0;j < EquationsNum;j++)
            params.push_back(result(j,count) + (3/2.0)*k1[j] - (9/2.0)*k3[j] + 6*k4[j]);

        /*step 5 compute k4i factors*/
        for(int i = 0;i < EquationsNum;i++){
            k5.push_back((h/3)*signature<EquationsNum>::compute(functions[i],params));
        }
    }

};
#endif // ROUNGE_KUTTA

