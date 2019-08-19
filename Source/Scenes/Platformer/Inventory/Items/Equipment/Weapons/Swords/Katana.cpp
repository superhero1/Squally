#include "Katana.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Objects/Platformer/Collectables/IOU.h"

#include "Resources/ObjectResources.h"

#include "Strings/Items/Equipment/Weapons/Swords/Katana.h"

using namespace cocos2d;

const std::string Katana::SaveKeyKatana = "katana";

Katana* Katana::create()
{
	Katana* instance = new Katana();

	instance->autorelease();

	return instance;
}

Katana::Katana() : super(CurrencyInventory::create({{ IOU::getIdentifier(), 2 }}), 3, 6)
{
}

Katana::~Katana()
{
}

Item* Katana::clone()
{
	return Katana::create();
}

std::string Katana::getItemName()
{
	return Katana::SaveKeyKatana;
}

LocalizedString* Katana::getString()
{
	return Strings::Items_Equipment_Weapons_Swords_Katana::create();
}

std::string Katana::getIconResource()
{
	return ObjectResources::Items_Equipment_Weapons_Swords_Katana;
}

std::string Katana::getSerializationKey()
{
	return Katana::SaveKeyKatana;
}
