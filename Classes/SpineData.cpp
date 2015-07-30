//
//  SpineData.cpp
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#include "SpineData.h"

using namespace cocos2d;

SpineData* SpineData::createWithData(const std::string& jsonFile, const std::string& atlasFile)
{
    auto data = new(std::nothrow) SpineData();
    if(data && data->initWithData(jsonFile, atlasFile))
    {
        data->autorelease();
        return data;
    }
    else
    {
        delete data;
        data = nullptr;
    }
    return nullptr;
}

SpineData::SpineData()
: _skelton(nullptr)
, _name("")
{

}

SpineData::~SpineData()
{
    CC_SAFE_RELEASE(_skelton);
}

bool SpineData::initWithData(const std::string& jsonFile, const std::string& atlasFile)
{
    if(!Node::init())
    {
        return false;
    }
    
    if(!FileUtils::getInstance()->isFileExist(jsonFile) ||
       !FileUtils::getInstance()->isFileExist(atlasFile))
    {
        return false;
    }
    
    _skelton = spine::SkeletonAnimation::createWithFile(jsonFile, atlasFile);
    if(!_skelton)
    {
        return false;
    }
    _skelton->retain();
    
    for(int i = 0; i < _skelton->getSkeleton()->data->animationsCount; ++i)
    {
        _animationNameList.emplace_back((*(_skelton->getSkeleton()->data->animations + i))->name);
    }
    
    return true;
}