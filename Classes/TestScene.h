//
//  TestScene.h
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#ifndef __SpineTester__TestScene__
#define __SpineTester__TestScene__

#include "cocos2d.h"
#include "SpineFileReader.h"
#include "SpineData.h"
#include "ui/CocosGUI.h"

class TestScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene *createScene();
    CREATE_FUNC(TestScene);
    virtual ~TestScene();
    void play();

    TestScene();
    bool init() override;
    
    static const cocos2d::Size ListViewItemSize;
    
private:
    bool _initInner();
    bool _initFileList();
    void _refreshUIAnimationList();
    void _refreshUIPlayList();
    void _addPlayList(const std::string& animationName);
    void _clearPlayList();
    
    cocos2d::ui::ListView* _uiFileList;
    cocos2d::ui::ListView* _uiAnimationList;
    cocos2d::ui::ListView* _uiPlayList;
    SpineData* _selectedData;
    
    std::vector<std::string> _playList;

    std::unique_ptr<SpineFileReader> _spineFileReader;
    int _playListItr;
    void _animationEndCallback();
    
    cocos2d::ui::Layout* _createLabelLayout(std::string text);
};

#endif /* defined(__SpineTester__TestScene__) */