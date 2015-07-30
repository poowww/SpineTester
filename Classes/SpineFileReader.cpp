//
//  SpineFileReader.cpp
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#include "SpineFileReader.h"

using namespace cocos2d;

const std::string SpineFileReader::SpineFileDirectory = "spine";
const std::string SpineFileReader::SpineFileListFileName = "list.txt";
const std::string SpineFileReader::JsonSuffix = ".json";
const std::string SpineFileReader::AtlasSuffix = ".atlas";

SpineFileReader* SpineFileReader::create()
{
    auto reader = new(std::nothrow) SpineFileReader();
    
    if(reader && reader->init())
    {
        return reader;
    }
    else
    {
        delete reader;
        reader = nullptr;
    }
    
    return nullptr;
}

SpineFileReader::SpineFileReader()
{

}

SpineFileReader::~SpineFileReader()
{

}

bool SpineFileReader::init()
{
    FileUtils::getInstance()->addSearchPath(SpineFileDirectory);

    if(!FileUtils::getInstance()->isFileExist(SpineFileListFileName))
    {
        return false;
    }
    
    auto row = FileUtils::getInstance()->getStringFromFile(SpineFileListFileName);
    std::istringstream stream(row);
    
    std::string field;
    while (std::getline(stream, field, '\n'))
    {
        _fileList.push_back(field);
    }
    
    for(const auto& file : _fileList)
    {
        auto data = SpineData::createWithData(file + JsonSuffix, file + AtlasSuffix);
        if(data)
        {
            data->setName(file);
            _spineDataList.pushBack(data);
        }
        else
        {
            CCLOGWARN("SpineFileReader::init : file %s cannot read.", file.c_str());
        }
    }
    
    return _spineDataList.size() != 0;
}