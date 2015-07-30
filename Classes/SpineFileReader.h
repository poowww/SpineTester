//
//  SpineFileReader.h
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#ifndef __SpineTester__SpineFileReader__
#define __SpineTester__SpineFileReader__

#include "cocos2d.h"
#include "SpineData.h"

class SpineFileReader
{
public:
    static SpineFileReader* create();
    static const std::string SpineFileDirectory;
    static const std::string SpineFileListFileName;
    
    cocos2d::Vector<SpineData*>& getSpineDataList() { return _spineDataList; }
    virtual ~SpineFileReader();

protected:
    SpineFileReader();
    virtual bool init();

private:
    std::vector<std::string> _fileList;
    cocos2d::Vector<SpineData*> _spineDataList;
    
    static const std::string JsonSuffix;
    static const std::string AtlasSuffix;
};

#endif /* defined(__SpineTester__SpineFileReader__) */