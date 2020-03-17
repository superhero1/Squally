#include "Binary8.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Binary8::SaveKeyBinary8 = "binary-8";

Binary8* Binary8::create()
{
	Binary8* instance = new Binary8();

	instance->autorelease();

	return instance;
}

Binary8::Binary8() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 1 }}), ItemMeta(3, 3))
{
}

Binary8::~Binary8()
{
}

Item* Binary8::clone()
{
	return Binary8::create();
}

std::string Binary8::getItemName()
{
	return Binary8::SaveKeyBinary8;
}

LocalizedString* Binary8::getString()
{
	return Strings::Hexus_Cards_Binary8::create();
}

std::string Binary8::getIconResource()
{
	return ItemResources::Collectables_Cards_CardBinary;
}

std::string Binary8::getSerializationKey()
{
	return Binary8::SaveKeyBinary8;
}

std::string Binary8::getCardKey()
{
	return CardKeys::Binary8;
}
