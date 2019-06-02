﻿#include "FloatingPointSelectPage.h"

#include "cocos/2d/CCSprite.h"

#include "Engine/Events/HackableEvents.h"
#include "Engine/Hackables/CodeEditor/Lexicon/Pages/LexiconPages.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"

#include "Resources/UIResources.h"

#include "Strings/Hacking/Lexicon/Pages/ChapterSelect/ChooseAnOperation.h"
#include "Strings/Hacking/Lexicon/Instructions/ControlFlow/Nop.h"

using namespace cocos2d;

const std::string FloatingPointSelectPage::Identifier = "FLOATING_POINT_SELECT_PAGE";

FloatingPointSelectPage* FloatingPointSelectPage::create()
{
	FloatingPointSelectPage* instance = new FloatingPointSelectPage();

	instance->autorelease();

	return instance;
}

FloatingPointSelectPage::FloatingPointSelectPage() : super(FloatingPointSelectPage::Identifier, PageType::Right)
{
	this->decorLine = Sprite::create(UIResources::Menus_LexiconMenu_DecorLine);

	this->operationSelectLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H2, Strings::Hacking_Lexicon_Pages_ChapterSelect_ChooseAnOperation::create());
	this->nopOperationButton = this->buildInstructionLabel(Strings::Hacking_Lexicon_Instructions_ControlFlow_Nop::create(), NopPage::Identifier);
	
	this->operationSelectLabel->setTextColor(super::TextColor);

	this->addChild(this->decorLine);
	this->addChild(this->operationSelectLabel);
	this->addChild(this->nopOperationButton);
}

FloatingPointSelectPage::~FloatingPointSelectPage()
{
}

void FloatingPointSelectPage::initializePositions()
{
	super::initializePositions();

	const float vOffset = -48.0f;
	const float vSpacing = -144.0f;

	this->decorLine->setPosition(Vec2(0.0f, super::PageSize.height / 2.0f + vOffset));
	this->operationSelectLabel->setPosition(Vec2(0.0f, super::PageSize.height / 2.0f + vOffset - 56.0f));
	this->nopOperationButton->setPosition(Vec2(0.0f, super::PageSize.height / 2.0f + vOffset - 128.0f + vSpacing * 0.0f));
}

void FloatingPointSelectPage::initializeListeners()
{
	super::initializeListeners();
}
