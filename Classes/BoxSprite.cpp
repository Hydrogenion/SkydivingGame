//
//  BoxSprite.cpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/21.
//

#include "BoxSprite.hpp"
#include <math.h>
BoxSprite::BoxSprite()
{
#if CC_SPRITE_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif //CC_SPRITE_DEBUG_DRAW
}

bool Lagrange(std::vector<double> funcInfo, std::vector<std::vector<double> > soldierInfo)
{
    std::vector<int> x(soldierInfo[0].begin(), soldierInfo[0].end());
    std::vector<int> y(soldierInfo[1].begin(), soldierInfo[1].end());
    double fix_x = 0;
    double result = 0;
    for (int i = 0; i < x.size(); ++i){
        double temp = y[i];
        for (int j = 0; j < x.size(); ++j){
            if (i != j){
                temp = temp * (fix_x - x[j]);
                temp = temp / (x[i] - x[j]);
            }
        }
        result += temp;
    }
    if (abs(result - funcInfo[0])<0.5){
        return true;
    }else{
        return false;
    }
}

double DifferenceQuotient(int n, std::vector<double>& x, std::vector<double>& y){
    double f = 0;
    double temp = 0;
    for (int i = 0; i < n+1; ++i){
        temp = y[i];
        for (int j = 0; j < n+1; ++j){
            if (i != j){
                temp /= (x[i] - x[j]);
            }
        }
        f += temp;
    }
    return f;
}

bool Newton(std::vector<double> funcInfo, std::vector<std::vector<double> > soldierInfo){
    std::vector<double> x(soldierInfo[0].begin(), soldierInfo[0].end());
    std::vector<double> y(soldierInfo[1].begin(), soldierInfo[1].end());
//    for (int i=0; i<x.size(); ++i){
//        cout << x[i]<<endl;
//    }
    double result = 0;
    double fix_x = 0;
    for (int i = 0; i < x.size(); ++i){
        double temp = 1;
        double f = DifferenceQuotient(i, x, y);
        for (int j = 0; j < i; ++j){
            temp = temp * (fix_x - x[j]);
        }
        result += f * temp;
    }
    if (abs(result - funcInfo[0])<0.5){
        return true;
    }
    else {
        return false;
    }
}

BoxSprite* BoxSprite::create(const std::string& filename, const Rect& rect)
{
    BoxSprite *sprite = new (std::nothrow) BoxSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}

bool OpenBox(std::vector<double> funcInfo, std::vector<std::vector<double> > soldierInfo, std::string type){
    if (soldierInfo[0].size() < funcInfo[1] + 1){
//        info_print("the number of soldier is too small");
        return false;
    }else if (type == "L"){
        return Lagrange(funcInfo, soldierInfo);
    }else{
        return Newton(funcInfo, soldierInfo);
    }
}

//输入是叫过来的战士们的（id+1）数组
bool BoxSprite::isOpen(std::vector<double> idInfo, std::vector<double> shareKeyInfo) {
    std::vector<std::vector<double> > soldierInfo;
    soldierInfo.push_back(idInfo);
    soldierInfo.push_back(shareKeyInfo);
    std::vector<double> funcInfo;
    funcInfo.push_back(coefficientInfo[0]);
    funcInfo.push_back(coefficientInfo.size()-1);
    return OpenBox(funcInfo, soldierInfo, "L");
}


double BoxSprite::generateShareKey(double id){
    double y = 0.0;
    for (int j=0; j < coefficientInfo.size(); ++j){
        y += coefficientInfo[j] * pow(id, j);
    }
    return y;
}
