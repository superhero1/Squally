#include "NpcBase.h"

#include "Engine/Input/ClickableNode.h"
#include "Engine/Physics/CollisionObject.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

NpcBase::NpcBase(
	ValueMap& properties,
	std::string entityName,
	std::string scmlResource,
	std::string emblemResource,
	Size size,
	float scale,
	Vec2 collisionOffset
) : super(
	properties,
	entityName,
	scmlResource,
	emblemResource,
	size,
	scale,
	collisionOffset)
{
	this->chatBubbleSprite = Sprite::create(UIResources::Platformer_ChatBubble);

	this->interactButton = ClickableNode::create(Sprite::create(), Sprite::create());
	this->interactButton->setMouseClickCallback(CC_CALLBACK_0(NpcBase::onInteractButtonClick, this));

	this->chatBubbleSprite->setPosition(Vec2(0.0f, 196.0f));
	this->chatBubbleSprite->setVisible(false);

	this->addChild(this->chatBubbleSprite);
	this->addChild(this->interactButton);
}

NpcBase::~NpcBase()
{
}

void NpcBase::update(float dt)
{
	super::update(dt);
}

void NpcBase::onInteractButtonClick()
{
	//// HexusEvents::startGame(HexusEvents::HexusGameEventArgs(this->deck, this->deck));
}
