#include "Decimal1.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Decimal1::SaveKeyDecimal1 = "decimal-1";

Decimal1* Decimal1::create()
{
	Decimal1* instance = new Decimal1();

	instance->autorelease();

	return instance;
}

Decimal1::Decimal1() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 1 }}), ItemMeta(3, 3))
{
}

Decimal1::~Decimal1()
{
}

Item* Decimal1::clone()
{
	return Decimal1::create();
}

std::string Decimal1::getItemName()
{
	return Decimal1::SaveKeyDecimal1;
}

LocalizedString* Decimal1::getString()
{
	return Strings::Hexus_Cards_Decimal1::create();
}

std::string Decimal1::getIconResource()
{
	return ItemResources::Collectables_Cards_CardDecimal;
}

std::string Decimal1::getSerializationKey()
{
	return Decimal1::SaveKeyDecimal1;
}

std::string Decimal1::getCardKey()
{
	return CardKeys::Decimal1;
}
