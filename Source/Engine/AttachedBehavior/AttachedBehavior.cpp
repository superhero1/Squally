#include "AttachedBehavior.h"

#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"

#include "Engine/Events/ObjectEvents.h"

using namespace cocos2d;

AttachedBehavior::AttachedBehavior(GameObject* owner, std::string attachedBehaviorArgs) : super()
{
	this->attachedBehaviorArgs = attachedBehaviorArgs;
	this->owner = owner;

	if (this->owner == nullptr)
	{
		this->invalidate();
	}
}

AttachedBehavior::~AttachedBehavior()
{
	this->invalidated = false;
}

void AttachedBehavior::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();

	if (!this->invalidated)
	{
		this->onLoad();
	}
}

void AttachedBehavior::initializeListeners()
{
	super::initializeListeners();
}

void AttachedBehavior::listenForStateWrite(std::string key, std::function<void(cocos2d::Value)> onWrite)
{
	const std::string eventKey = key + "_" + this->owner->getUniqueIdentifier();

	this->addEventListenerIgnorePause(EventListenerCustom::create(ObjectEvents::EventWriteStatePrefix + eventKey, [=](EventCustom* eventCustom)
	{
		ObjectEvents::StateWriteArgs* args = static_cast<ObjectEvents::StateWriteArgs*>(eventCustom->getUserData());
		
		if (args != nullptr)
		{
			onWrite(args->value);
		}
	}));
}

std::string AttachedBehavior::getAttachedBehaviorArgs()
{
	return this->attachedBehaviorArgs;
}

void AttachedBehavior::invalidate()
{
	this->invalidated = true;
}

bool AttachedBehavior::isInvalidated()
{
	return this->invalidated;
}