#include "HexusOpponentScarecrow.h"

const std::string HexusOpponentScarecrow::OpponentSaveKey = "HEXUS_OPPONENT_SAVE_KEY_SCARECROW";
HexusOpponentScarecrow* HexusOpponentScarecrow::instance = nullptr;

HexusOpponentScarecrow* HexusOpponentScarecrow::getInstance()
{
	if (HexusOpponentScarecrow::instance == nullptr)
	{
		HexusOpponentScarecrow::instance = new HexusOpponentScarecrow();
	}

	return HexusOpponentScarecrow::instance;
}

HexusOpponentScarecrow::HexusOpponentScarecrow() : HexusOpponentData(
	Resources::Entities_Platformer_Environment_Castle_Enemies_Scarecrow_Animations,
	Resources::Menus_MinigamesMenu_Hexus_HexusFrameCastle,
	1.0f,
	Vec2(-32.0f, -96.0f),
	Vec2(-16.0f, -128.0f),
	HexusOpponentScarecrow::OpponentSaveKey,
	HexusOpponentData::Difficulty::Stupid,
	Card::CardStyle::Shadow,
	HexusOpponentData::generateReward(0.585f),
	HexusOpponentData::generateDeck(25, 0.585f,
		{
			CardList::getInstance()->cardListByName->at(CardKeys::Addition),
			CardList::getInstance()->cardListByName->at(CardKeys::LogicalAnd),
			CardList::getInstance()->cardListByName->at(CardKeys::LogicalOr),
			CardList::getInstance()->cardListByName->at(CardKeys::LogicalXor),
			CardList::getInstance()->cardListByName->at(CardKeys::ShiftLeft),
			CardList::getInstance()->cardListByName->at(CardKeys::ShiftRight),
		})
	)
{
}

HexusOpponentScarecrow::~HexusOpponentScarecrow()
{
}