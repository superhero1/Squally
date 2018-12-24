#include "OptionsScene.h"

#include "cocos/2d/CCNode.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Engine/GlobalDirector.h"
#include "Events/NavigationEvents.h"
#include "Menus/Options/OptionsMenu.h"
#include "Menus/MenuBackground.h"

using namespace cocos2d;

OptionsScene* OptionsScene::instance;

void OptionsScene::registerGlobalScene()
{
	if (OptionsScene::instance == nullptr)
	{
		OptionsScene::instance = new OptionsScene();

		OptionsScene::instance->autorelease();
		OptionsScene::instance->initializeListeners();
	}

	GlobalDirector::registerGlobalScene(OptionsScene::instance);
}

OptionsScene::OptionsScene()
{
	this->menu = OptionsMenu::create();
	this->backgroundNode = Node::create();

	this->addChild(this->backgroundNode);
	this->addChild(this->menu);

	this->setFadeSpeed(0.0f);
}

OptionsScene::~OptionsScene()
{
}

void OptionsScene::onEnter()
{
	GlobalScene::onEnter();

	this->backgroundNode->addChild(MenuBackground::claimInstance());
}

void OptionsScene::initializePositions()
{
	GlobalScene::initializePositions();
}

void OptionsScene::initializeListeners()
{
	GlobalScene::initializeListeners();

	OptionsScene::instance->addGlobalEventListener(EventListenerCustom::create(NavigationEvents::EventNavigateOptions, [](EventCustom* args)
	{
		GlobalDirector::loadScene(OptionsScene::instance);
	}));

	this->menu->setBackClickCallback([]()
	{
		NavigationEvents::navigateBack();
	});
}
