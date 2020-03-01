#include "MayanGemPurpleItem.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ObjectResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string MayanGemPurpleItem::SaveKeyMayanGemPurpleItem = "mayan-gem-purple";

MayanGemPurpleItem* MayanGemPurpleItem::create()
{
	MayanGemPurpleItem* instance = new MayanGemPurpleItem();

	instance->autorelease();

	return instance;
}

MayanGemPurpleItem::MayanGemPurpleItem() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 0 }}))
{
}

MayanGemPurpleItem::~MayanGemPurpleItem()
{
}

Item* MayanGemPurpleItem::clone()
{
	return MayanGemPurpleItem::create();
}

std::string MayanGemPurpleItem::getItemName()
{
	return MayanGemPurpleItem::SaveKeyMayanGemPurpleItem;
}

LocalizedString* MayanGemPurpleItem::getString()
{
	return Strings::Items_Misc_Keys_TownKey::create();
}

std::string MayanGemPurpleItem::getIconResource()
{
	return ObjectResources::Doors_Mayan_PurpleGem;
}

std::string MayanGemPurpleItem::getSerializationKey()
{
	return MayanGemPurpleItem::SaveKeyMayanGemPurpleItem;
}

