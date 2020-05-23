//
//  BoxSprite.hpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/21.
//

#ifndef BoxSprite_hpp
#define BoxSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class BoxSprite : public Sprite{
public:
    BoxSprite();
    static BoxSprite* create(const std::string& filename, const Rect& rect);
    void setType(std::string _type){type = _type;}
    void setCoefficientInfo(std::vector<double> _coefficientInfo) {coefficientInfo = _coefficientInfo;}
    bool isOpen(std::vector<double> idInfo, std::vector<double> shareKeyInfo);
    double generateShareKey(double id);
private:
    //插值类型，'L':拉格朗日插值，'N':牛顿插值
    std::string type;
    //多项式函数系数
    std::vector<double> coefficientInfo;

};
#endif /* BoxSprite_hpp */
