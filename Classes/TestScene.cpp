//
//  TestScene.cpp
//  SpineTester
//
//  Created by ShotaroTakagi on 2015/07/28.
//
//

#include "TestScene.h"
#include "extensions/cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

const Size TestScene::ListViewItemSize(120,30);

Scene* TestScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TestScene::create();
    scene->addChild(layer);

    return scene;
}

TestScene::TestScene()
: _spineFileReader(nullptr)
, _uiFileList(nullptr)
, _uiAnimationList(nullptr)
, _uiPlayList(nullptr)
, _selectedData(nullptr)
, _playListItr(0)
{

}

TestScene::~TestScene()
{

}

bool TestScene::init()
{
    if ( !LayerColor::initWithColor(Color4B::BLACK) )
    {
        return false;
    }
    _spineFileReader = std::unique_ptr<SpineFileReader>(SpineFileReader::create());
    
    if(_spineFileReader)
    {
        return _initInner();
    }
    else
    {
        //no spine file
    }

    return true;
}

bool TestScene::_initInner()
{
    //init ui
    _uiFileList = ListView::create();
    _uiAnimationList = ListView::create();
    _uiPlayList = ListView::create();
    
    if(!_uiFileList ||
       !_uiAnimationList ||
       !_uiPlayList)
    {
        return false;
    }
    
    //init filelist
    if(!_initFileList())
    {
        return false;
    }
    
    addChild(_uiFileList);
    addChild(_uiAnimationList);
    addChild(_uiPlayList);
    
    _uiFileList->setTouchEnabled(true);
    _uiFileList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _uiFileList->setSwallowTouches(true);
    _uiAnimationList->setTouchEnabled(true);
    _uiAnimationList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _uiAnimationList->setSwallowTouches(true);
    _uiPlayList->setTouchEnabled(true);
    _uiPlayList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _uiPlayList->setSwallowTouches(true);
    
    _uiFileList->addEventListener([this](Ref* ref, ListView::EventType type)
    {
        log("filelist event listener enterd");
        if(type == ListView::EventType::ON_SELECTED_ITEM_END)
        {
            auto listView = static_cast<ListView*>(ref);
            auto selectedIndex = listView->getCurSelectedIndex();
            this->_selectedData = this->_spineFileReader->getSpineDataList().at(selectedIndex);
            this->_refreshUIAnimationList();
        }
    });
    
    _uiAnimationList->addEventListener([this](Ref* ref, ListView::EventType type)
    {
        if(type == ListView::EventType::ON_SELECTED_ITEM_END)
        {
            auto listView = static_cast<ListView*>(ref);
            auto selectedIndex = listView->getCurSelectedIndex();
            this->_playList.push_back(this->_selectedData->getAnimationNameList().at(selectedIndex) );
            this->_refreshUIPlayList();
        }
    });
    
    _uiPlayList->addEventListener([this](Ref* ref, ListView::EventType type)
    {
        if(type == ListView::EventType::ON_SELECTED_ITEM_END)
        {
            auto listView = static_cast<ListView*>(ref);
            auto selectedIndex = listView->getCurSelectedIndex();
            // If delete item in this frame, touch event propagates in deteled item.
            this->scheduleOnce([this, selectedIndex](float){this->_playList.erase(this->_playList.begin() + selectedIndex); this->_refreshUIPlayList();}, 0.001f, "delete anim");
        }
    });
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    log("visible size width %f, height %f", visibleSize.width, visibleSize.height);
    
    //init play list clear button
    auto clearLabel = Label::createWithSystemFont("clear", "fonts/arial.ttf", 20);
    clearLabel->setColor(Color3B::ORANGE);
    auto clearMenu = MenuItemLabel::create(clearLabel, [this](Ref*){ log("clear button touched");this->_clearPlayList(); });
    
    //init play button
    auto playLabel = Label::createWithSystemFont("play", "fonts/arial.ttf", 20);
    playLabel->setColor(Color3B::ORANGE);
    auto playMenu = MenuItemLabel::create(playLabel, [this](Ref*){ log("play button touched");this->play(); });
    playMenu->setPosition(Vec2(0, - playLabel->getContentSize().height - 30));
    
    auto menu = Menu::create(clearMenu, playMenu, nullptr);
    menu->setContentSize(Size(200, 200));
    addChild(menu);
    menu->setPosition(Vec2(visibleSize.width * 3 / 4 + 40, visibleSize.height / 2));
    
    //adjust layout
    _uiFileList->setContentSize(Size(visibleSize.width / 4, visibleSize.height - 40));
    _uiAnimationList->setContentSize(Size(visibleSize.width / 4, visibleSize.height - 40));
    _uiPlayList->setContentSize(Size(visibleSize.width / 4, visibleSize.height - 40));

    _uiFileList->setPosition(Vec2::ZERO);
    _uiAnimationList->setPosition(Vec2(visibleSize.width / 4, 0));
    _uiPlayList->setPosition(Vec2(visibleSize.width * 2 / 4, 0));
    
    //labels for each list
    auto fileListLabel = Label::createWithSystemFont("files", "fonts/arial.ttf", 20);
    auto animListLabel = Label::createWithSystemFont("animations", "fonts/arial.ttf", 20);
    auto playListLabel = Label::createWithSystemFont("playlist", "fonts/arial.ttf", 20);
    fileListLabel->setColor(Color3B::RED);
    animListLabel->setColor(Color3B::RED);
    playListLabel->setColor(Color3B::RED);
    fileListLabel->setAnchorPoint(Vec2::ZERO);
    animListLabel->setAnchorPoint(Vec2::ZERO);
    playListLabel->setAnchorPoint(Vec2::ZERO);
    fileListLabel->setPosition(_uiFileList->getPosition().x, visibleSize.height - fileListLabel->getContentSize().height - 10);
    animListLabel->setPosition(_uiAnimationList->getPosition().x, visibleSize.height - animListLabel->getContentSize().height - 10);
    playListLabel->setPosition(_uiPlayList->getPosition().x, visibleSize.height - playListLabel->getContentSize().height - 10);
    addChild(fileListLabel);
    addChild(animListLabel);
    addChild(playListLabel);
    
    return true;
}

bool TestScene::_initFileList()
{
    _uiFileList->removeAllChildren();
    
    for(auto& ele : _spineFileReader->getSpineDataList() )
    {
        auto label = _createLabelLayout(ele->getName());
        _uiFileList->pushBackCustomItem(label);
    }
    
    return true;
}

void TestScene::play()
{
    if(_playList.size() == 0)
    {
        return;
    }
    
    _playListItr = 0;
    auto track = _selectedData->getSkelton()->setAnimation(0, _playList.at(_playListItr), false);
    _selectedData->getSkelton()->setTrackCompleteListener(track, [this](int,int)
    {
        this->_animationEndCallback();
    });
    
    auto layer = LayerColor::create(Color4B(128, 128, 128, 192));
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch*, Event*){ return true; };
    listener->onTouchEnded = [this, layer](Touch*, Event*)
    {
        _selectedData->getSkelton()->clearTracks();
        _selectedData->getSkelton()->removeFromParent();
        layer->removeFromParentAndCleanup(true);
    };
    listener->setSwallowTouches(true);
    layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layer);
    addChild(layer, 1000);
    layer->addChild(_selectedData->getSkelton());
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    _selectedData->getSkelton()->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
}

void TestScene::_refreshUIAnimationList()
{
    _uiAnimationList->removeAllItems();

    if(!_selectedData)
    {
        return;
    }
    
    for(auto& ele : _selectedData->getAnimationNameList())
    {
        auto label = _createLabelLayout(ele);
        _uiAnimationList->pushBackCustomItem(label);
    }
    
    _clearPlayList();
}

void TestScene::_refreshUIPlayList()
{
    _uiPlayList->removeAllItems();
    
    for(auto itr = _playList.begin(); itr != _playList.end(); ++itr)
    {
        auto label = _createLabelLayout((*itr));
        _uiPlayList->pushBackCustomItem(label);
    }
}

void TestScene::_addPlayList(const std::string& animationName)
{
    _playList.push_back(animationName);
    _refreshUIPlayList();
}

void TestScene::_clearPlayList()
{
    _playList.clear();
    _refreshUIPlayList();
}

Layout* TestScene::_createLabelLayout(std::string text)
{
    auto label = Label::createWithSystemFont(text, "fonts/arial.ttf", 15);
    
    auto layout = Layout::create();
    layout->setContentSize(ListViewItemSize);
    layout->addChild(label);
    label->setPosition(ListViewItemSize / 2);
    layout->setTouchEnabled(true);
    
    return layout;
}

void TestScene::_animationEndCallback()
{
    ++_playListItr;
    if(_playListItr >= _playList.size())
    {
        _playListItr = 0;
    }
    
    auto track = _selectedData->getSkelton()->setAnimation(0, _playList.at(_playListItr), false);
    this->_selectedData->getSkelton()->setTrackCompleteListener(track, [this](int,int)
    {
        this->_animationEndCallback();
    });
}
