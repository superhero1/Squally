#include "SwordExecute.h"

#include "Events/CombatEvents.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Sound/WorldSound.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const float SwordExecute::SwordExecuteBonus = 1.4f;

SwordExecute* SwordExecute::create(int damageMin, int damageMax, float attackDuration, float recoverDuration, float priority)
{
	SwordExecute* instance = new SwordExecute(damageMin, damageMax, attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

SwordExecute::SwordExecute(int damageMin, int damageMax, float attackDuration, float recoverDuration, float priority)
	: super(AttackType::Damage, UIResources::Menus_Icons_BleedingLimb, priority, int(float(damageMin) * SwordExecute::SwordExecuteBonus), int(float(damageMax) * SwordExecute::SwordExecuteBonus), 4, attackDuration, recoverDuration)
{
	this->slashSound = WorldSound::create(SoundResources::Platformer_Combat_Attacks_Physical_Swings_Swing1);
	this->hitSound = WorldSound::create(SoundResources::Platformer_Combat_Attacks_Physical_Impact_HitSoft1);

	this->addChild(this->slashSound);
	this->addChild(this->hitSound);
}

SwordExecute::~SwordExecute()
{
}

PlatformerAttack* SwordExecute::cloneInternal()
{
	return SwordExecute::create(this->getBaseDamageMin(), this->getBaseDamageMax(), this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* SwordExecute::getString()
{
	return Strings::Platformer_Combat_Attacks_Execute::create();
}

std::string SwordExecute::getAttackAnimation()
{
	return "AttackSlash";
}

void SwordExecute::onAttackTelegraphBegin()
{
	super::onAttackTelegraphBegin();
	
	this->slashSound->play(false, this->attackDuration / 2.0f);
}

void SwordExecute::performAttack(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->doDamageOrHealing(owner, target);
}

void SwordExecute::doDamageOrHealing(PlatformerEntity* owner, PlatformerEntity* target)
{
	this->hitSound->play();
	CombatEvents::TriggerDamage(CombatEvents::DamageOrHealingArgs(owner, target, this->getRandomDamage()));

	GameCamera::getInstance()->shakeCamera(0.2f, 12.0f, 0.3f);
}
