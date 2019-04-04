#include "CardData.h"

#include "Engine/Localization/LocalizedString.h"
#include "Scenes/Hexus/CardData/CardKeys.h"

#include "Strings/Generics/Empty.h"
#include "Strings/Hexus/Cards/Abbreviations/ADD.h"
#include "Strings/Hexus/Cards/Abbreviations/AND.h"
#include "Strings/Hexus/Cards/Abbreviations/BIN.h"
#include "Strings/Hexus/Cards/Abbreviations/CSHL.h"
#include "Strings/Hexus/Cards/Abbreviations/CSHR.h"
#include "Strings/Hexus/Cards/Abbreviations/DEC.h"
#include "Strings/Hexus/Cards/Abbreviations/DIV.h"
#include "Strings/Hexus/Cards/Abbreviations/ERR.h"
#include "Strings/Hexus/Cards/Abbreviations/FLIP1.h"
#include "Strings/Hexus/Cards/Abbreviations/FLIP2.h"
#include "Strings/Hexus/Cards/Abbreviations/FLIP3.h"
#include "Strings/Hexus/Cards/Abbreviations/FLIP4.h"
#include "Strings/Hexus/Cards/Abbreviations/HEX.h"
#include "Strings/Hexus/Cards/Abbreviations/INV.h"
#include "Strings/Hexus/Cards/Abbreviations/MOD.h"
#include "Strings/Hexus/Cards/Abbreviations/MOV.h"
#include "Strings/Hexus/Cards/Abbreviations/MUL.h"
#include "Strings/Hexus/Cards/Abbreviations/OR.h"
#include "Strings/Hexus/Cards/Abbreviations/SHL.h"
#include "Strings/Hexus/Cards/Abbreviations/SHR.h"
#include "Strings/Hexus/Cards/Abbreviations/SUB.h"
#include "Strings/Hexus/Cards/Abbreviations/XOR.h"

CardData::CardData(std::string cardResourceFile, std::string cardKey, CardType cardType, unsigned int attack)
{
	this->cardResourceFile = cardResourceFile;
	this->cardKey = cardKey;
	this->cardType = cardType;
	this->attack = attack;
}

CardData::~CardData()
{
}

LocalizedString* CardData::getCardTypeString()
{
	switch (this->cardType)
	{
		case CardData::CardType::Binary:
		{
			return Strings::Hexus_Cards_Abbreviations_BIN::create();
		}
		case CardData::CardType::Decimal:
		{
			return Strings::Hexus_Cards_Abbreviations_DEC::create();
		}
		case CardData::CardType::Hexidecimal:
		{
			return Strings::Hexus_Cards_Abbreviations_HEX::create();
		}
		case CardData::CardType::Special_MOV:
		{
			return Strings::Hexus_Cards_Abbreviations_MOV::create();
		}
		case CardData::CardType::Special_AND:
		{
			return Strings::Hexus_Cards_Abbreviations_AND::create();
		}
		case CardData::CardType::Special_OR:
		{
			return Strings::Hexus_Cards_Abbreviations_OR::create();
		}
		case CardData::CardType::Special_XOR:
		{
			return Strings::Hexus_Cards_Abbreviations_XOR::create();
		}
		case CardData::CardType::Special_SHL:
		{
			return Strings::Hexus_Cards_Abbreviations_SHL::create();
		}
		case CardData::CardType::Special_SHR:
		{
			return Strings::Hexus_Cards_Abbreviations_SHR::create();
		}
		case CardData::CardType::Special_INV:
		{
			return Strings::Hexus_Cards_Abbreviations_INV::create();
		}
		case CardData::CardType::Special_FLIP1:
		{
			return Strings::Hexus_Cards_Abbreviations_FLIP1::create();
		}
		case CardData::CardType::Special_FLIP2:
		{
			return Strings::Hexus_Cards_Abbreviations_FLIP2::create();
		}
		case CardData::CardType::Special_FLIP3:
		{
			return Strings::Hexus_Cards_Abbreviations_FLIP3::create();
		}
		case CardData::CardType::Special_FLIP4:
		{
			return Strings::Hexus_Cards_Abbreviations_FLIP4::create();
		}
		case CardData::CardType::Special_ADD:
		{
			return Strings::Hexus_Cards_Abbreviations_ADD::create();
		}
		case CardData::CardType::Special_SUB:
		{
			return Strings::Hexus_Cards_Abbreviations_SUB::create();
		}
		case CardData::CardType::Special_CLEAR:
		case CardData::CardType::Special_SUDDEN_DEATH:
		case CardData::CardType::Special_GREED:
		case CardData::CardType::Special_ABSORB:
		case CardData::CardType::Special_KILL:
		case CardData::CardType::Special_RETURN_TO_HAND:
		case CardData::CardType::Special_STEAL:
		case CardData::CardType::Special_BONUS_MOVES:
		case CardData::CardType::Special_PEEK:
		case CardData::CardType::Special_HEAL:
		case CardData::CardType::Special_POISON:
		case CardData::CardType::Special_DRANK:
		{
			return Strings::Generics_Empty::create();
		}
		default:
		{
			return Strings::Hexus_Cards_Abbreviations_ERR::create();
		}
	}
}

bool CardData::isAttackCard()
{
	switch (this->cardType)
	{
		case CardData::CardType::Binary:
		case CardData::CardType::Decimal:
		case CardData::CardType::Hexidecimal:
		case CardData::CardType::Special_ABSORB:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

bool CardData::isSpecialCard()
{
	return !this->isAttackCard();
}

bool CardData::isMultiTargetCard()
{
	switch (this->cardType)
	{
		case CardData::CardType::Special_SHL:
		case CardData::CardType::Special_SHR:
		case CardData::CardType::Special_HEAL:
		case CardData::CardType::Special_POISON:
		case CardData::CardType::Special_DRANK:
		case CardData::CardType::Special_FLIP1:
		case CardData::CardType::Special_FLIP2:
		case CardData::CardType::Special_FLIP3:
		case CardData::CardType::Special_FLIP4:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}

unsigned int CardData::getIntrinsicImmediate()
{
	switch (this->cardType)
	{
		case CardData::CardType::Binary:
		case CardData::CardType::Decimal:
		case CardData::CardType::Hexidecimal:
		case CardData::CardType::Special_CLEAR:
		case CardData::CardType::Special_SUDDEN_DEATH:
		{
			return this->attack;
		}
		case CardData::CardType::Special_BONUS_MOVES:
		{
			return 3;
		}
		case CardData::CardType::Special_FLIP1:
		{
			return 0b0001;
		}
		case CardData::CardType::Special_FLIP2:
		{
			return 0b0010;
		}
		case CardData::CardType::Special_FLIP3:
		{
			return 0b0100;
		}
		case CardData::CardType::Special_FLIP4:
		{
			return 0b1000;
		}
		case CardData::CardType::Special_HEAL:
		case CardData::CardType::Special_POISON:
		case CardData::CardType::Special_DRANK:
		{
			return this->attack / 2;
		}
		case CardData::CardType::Special_SHL:
		case CardData::CardType::Special_SHR:
		{
			return 0b0001;
		}
		case CardData::CardType::Special_MOV:
		case CardData::CardType::Special_AND:
		case CardData::CardType::Special_OR:
		case CardData::CardType::Special_XOR:
		case CardData::CardType::Special_INV:
		case CardData::CardType::Special_ADD:
		case CardData::CardType::Special_SUB:
		case CardData::CardType::Special_GREED:
		case CardData::CardType::Special_ABSORB:
		case CardData::CardType::Special_KILL:
		case CardData::CardType::Special_RETURN_TO_HAND:
		case CardData::CardType::Special_STEAL:
		case CardData::CardType::Special_PEEK:
		default:
		{
			return 0;
		}
	}
}
