#include "ToolBox.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Localization/LocalizedLabel.h"
#include "Scenes/Cipher/Config.h"
#include "Scenes/Cipher/Components/Blocks/AddBlock.h"
#include "Scenes/Cipher/Components/Blocks/AndBlock.h"
#include "Scenes/Cipher/Components/Blocks/BlockBase.h"
#include "Scenes/Cipher/Components/Blocks/BruteForceBlock.h"
#include "Scenes/Cipher/Components/Blocks/CshlBlock.h"
#include "Scenes/Cipher/Components/Blocks/CshrBlock.h"
#include "Scenes/Cipher/Components/Blocks/DivBlock.h"
#include "Scenes/Cipher/Components/Blocks/ImmediateBlock.h"
#include "Scenes/Cipher/Components/Blocks/InvBlock.h"
#include "Scenes/Cipher/Components/Blocks/ModBlock.h"
#include "Scenes/Cipher/Components/Blocks/MulBlock.h"
#include "Scenes/Cipher/Components/Blocks/OrBlock.h"
#include "Scenes/Cipher/Components/Blocks/ShlBlock.h"
#include "Scenes/Cipher/Components/Blocks/ShrBlock.h"
#include "Scenes/Cipher/Components/Blocks/SubBlock.h"
#include "Scenes/Cipher/Components/Blocks/XorBlock.h"
#include "Scenes/Cipher/CipherState.h"

#include "Resources/CipherResources.h"

#include "Strings/Cipher/Tools.h"

using namespace cocos2d;

ToolBox* ToolBox::create()
{
	ToolBox* instance = new ToolBox();

	instance->autorelease();

	return instance;
}

ToolBox::ToolBox()
{
	this->cipherToolsLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H1, Strings::Cipher_Tools::create());
	this->addBlock = AddBlock::create(true);
	this->andBlock = AndBlock::create(true);
	this->bruteForceBlock = BruteForceBlock::create(true);
	this->cshlBlock = CshlBlock::create(true);
	this->cshrBlock = CshrBlock::create(true);
	this->divBlock = DivBlock::create(true);
	this->immediateBlock = ImmediateBlock::create(true);
	this->invBlock = InvBlock::create(true);
	this->modBlock = ModBlock::create(true);
	this->mulBlock = MulBlock::create(true);
	this->orBlock = OrBlock::create(true);
	this->shlBlock = ShlBlock::create(true);
	this->shrBlock = ShrBlock::create(true);
	this->subBlock = SubBlock::create(true);
	this->xorBlock = XorBlock::create(true);

	this->blocks = std::vector<BlockBase*>(
	{
		this->andBlock,
		this->orBlock,
		this->xorBlock,
		this->modBlock,

		this->addBlock,
		this->subBlock,
		this->divBlock,
		this->mulBlock,

		this->shlBlock,
		this->shrBlock,
		this->cshlBlock,
		this->cshrBlock,

		this->invBlock,
		this->bruteForceBlock,
		this->immediateBlock,
	});

	this->cipherToolsLabel->enableShadow(Color4B::BLACK, Size(2, -2), 2);
	
	for (auto it = this->blocks.begin(); it != this->blocks.end(); it++)
	{
		this->addChild(*it);
	}

	this->addChild(this->cipherToolsLabel);
}

ToolBox::~ToolBox()
{
}

void ToolBox::onEnter()
{
	super::onEnter();
}

void ToolBox::initializePositions()
{
	super::initializePositions();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int index = 0;

	this->cipherToolsLabel->setPosition(Vec2(visibleSize.width / 2.0f + Config::RightColumnCenter, visibleSize.height / 2.0f + 450.0f));

	for (auto it = this->blocks.begin(); it != this->blocks.end(); it++, index++)
	{
		int x = index % 4;
		int y = index / 4;

		(*it)->setPosition(Vec2(visibleSize.width / 2.0f + Config::RightColumnCenter + (float(x) - 1.5f) * 80.0f, visibleSize.height / 2.0f + 336.0f - float(y) * 72.0f));
	}

	this->immediateBlock->setPosition(Vec2(visibleSize.width / 2.0f + Config::RightColumnCenter + (float(3) - 1.5f - 0.5f) * 80.0f, visibleSize.height / 2.0f + 336.0f - float(3) * 72.0f));
}

void ToolBox::onBeforeStateChange(CipherState* cipherState)
{
	super::onBeforeStateChange(cipherState);
}

void ToolBox::onAnyStateChange(CipherState* cipherState)
{
	super::onAnyStateChange(cipherState);
}