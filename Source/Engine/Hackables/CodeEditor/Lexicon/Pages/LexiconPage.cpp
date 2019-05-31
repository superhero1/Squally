﻿#include "LexiconPage.h"

#include "cocos/2d/CCDrawNode.h"

using namespace cocos2d;

const Size LexiconPage::TotalPageSize = Size(580.0f, 832.0f);
const Size LexiconPage::PageMargin = Size(32.0f, 32.0f);
const Size LexiconPage::PageSize = LexiconPage::TotalPageSize - LexiconPage::PageMargin * 2.0f;
const Vec2 LexiconPage::ChapterMarkerLocation = Vec2(-LexiconPage::PageSize.width / 2.0f + 64.0f, LexiconPage::PageSize.height / 2.0f - 192.0f);
const Vec2 LexiconPage::ChapterLocation = LexiconPage::ChapterMarkerLocation + Vec2(32.0f, 32.0f);
const Color4B LexiconPage::TextColor = Color4B(62, 45, 32, 255);

LexiconPage::LexiconPage(std::string pageIdentifier)
{
    this->pageIdentifier = pageIdentifier;
    this->debugDrawNode = DrawNode::create();

    this->debugDrawNode->setVisible(false);

    this->debugDrawNode->drawSolidRect(-Vec2(LexiconPage::TotalPageSize / 2.0f), LexiconPage::TotalPageSize / 2.0f, Color4F(Color4B(255, 0, 0, 128)));
    this->debugDrawNode->drawSolidRect(-Vec2(LexiconPage::PageSize / 2.0f), LexiconPage::PageSize / 2.0f, Color4F(Color4B(0, 255, 0, 128)));

    this->addChild(this->debugDrawNode);
}

LexiconPage::~LexiconPage()
{
}

void LexiconPage::onEnter()
{
    super::onEnter();
}

void LexiconPage::initializeListeners()
{
    super::initializeListeners();
}

void LexiconPage::onDeveloperModeEnable()
{
    super::onDeveloperModeEnable();

    this->debugDrawNode->setVisible(true);
}

void LexiconPage::onDeveloperModeDisable()
{
    super::onDeveloperModeDisable();

    this->debugDrawNode->setVisible(false);
}
