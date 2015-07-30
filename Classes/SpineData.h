//
//  SpineData.h
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#ifndef __SpineTester__SpineData__
#define __SpineTester__SpineData__

#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"

class SpineData : public cocos2d::Node
{
public:
    static SpineData* createWithData(const std::string& jsonFile, const std::string& atlasFile);
    
    SpineData();
    virtual ~SpineData();
    
    bool initWithData(const std::string& jsonFile, const std::string& atlasFile);
    CC_SYNTHESIZE_RETAIN(spine::SkeletonAnimation*, _skelton, Skelton);
    
    std::string getName() const { return _name; }
    void setName(const std::string& name){ _name = name; }
    
    std::vector<std::string>& getAnimationNameList() { return _animationNameList; }

protected:

private:
    std::string _name;
    std::vector<std::string> _animationNameList;
    
};

#endif /* defined(__SpineTester__SpineData__) */