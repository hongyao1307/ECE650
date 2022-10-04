#ifndef A3GRAPH_H_
#define A3GRAPH_H_
#include "a3street.h"
#include <vector>
#include <string>

class graph
{   private:
    double k1,k2,b1,b2;
    int ifk;
    //double* formula[2];

    public:
    int x1,x2,x3,x4,y1,y2,y3,y4;
    std::vector <street> streets;
    int same_points1();//判断四个点是否完全一致
    int same_points2();
    int if_vertical1();//判断线短是否垂直
    int if_vertical2();
    void xy_formula1();//计算线短函数
    void xy_formula2();
    
    int inter_sect();//判断街道自身是否有相交
    int if_overlapping();//重叠
    int vertical_overlapping();

    
};
#endif