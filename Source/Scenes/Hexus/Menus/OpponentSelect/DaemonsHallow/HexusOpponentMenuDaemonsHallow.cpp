////////////////////////////////////////////////////////////////////////////////////////////
// THIS C++ FILE IS GENERATED DO NOT EDIT. RUN GenerateDataFiles.py TO GENERATE THIS FILE //
////////////////////////////////////////////////////////////////////////////////////////////

#include "HexusOpponentMenuDaemonsHallow.h"

#include "Engine/GlobalDirector.h"
#include "Engine/UI/Controls/ScrollPane.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/Asmodeus.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/CritterDemon.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonDragon.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonGhost.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonGrunt.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonRogue.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonShaman.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonSwordsman.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonWarrior.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/FireElemental.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/LavaGolem.h"
#include "Entities/Platformer/Helpers/DaemonsHallow/Apple.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Ash.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Brine.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Celeste.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Cindra.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Drak.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Igneus.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Lucifer.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Magnus.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Pan.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/PrincessMittens.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Ragnis.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Scaldor.h"
#include "Scenes/Hexus/Menus/OpponentSelect/HexusOpponentPreview.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/Asmodeus.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/CritterDemon.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonDragon.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonGhost.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonGrunt.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonRogue.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonShaman.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonSwordsman.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/DemonWarrior.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/FireElemental.h"
#include "Entities/Platformer/Enemies/DaemonsHallow/LavaGolem.h"
#include "Entities/Platformer/Helpers/DaemonsHallow/Apple.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Ash.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Brine.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Celeste.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Cindra.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Drak.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Igneus.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Lucifer.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Magnus.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Pan.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/PrincessMittens.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Ragnis.h"
#include "Entities/Platformer/Npcs/DaemonsHallow/Scaldor.h"
#include "Scenes/Hexus/Menus/ChapterSelect/DaemonsHallow/HexusChapterPreviewDaemonsHallow.h"

using namespace cocos2d;

HexusOpponentMenuDaemonsHallow* HexusOpponentMenuDaemonsHallow::instance = nullptr;

void HexusOpponentMenuDaemonsHallow::registerGlobalScene()
{
	if (HexusOpponentMenuDaemonsHallow::instance == nullptr)
	{
		HexusOpponentMenuDaemonsHallow::instance = new HexusOpponentMenuDaemonsHallow();

		HexusOpponentMenuDaemonsHallow::instance->autorelease();
		HexusOpponentMenuDaemonsHallow::instance->initializeListeners();
	}

	GlobalDirector::registerGlobalScene(HexusOpponentMenuDaemonsHallow::instance);
}

HexusOpponentMenuDaemonsHallow::HexusOpponentMenuDaemonsHallow() : super(NavigationEvents::NavigateHexusOpponentSelectArgs::Chapter::DaemonsHallow, HexusChapterPreviewDaemonsHallow::SaveKeyChapterName)
{
	this->opponents.push_back(HexusOpponentPreview::create(Asmodeus::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(CritterDemon::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonDragon::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonGhost::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonGrunt::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonRogue::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonShaman::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonSwordsman::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(DemonWarrior::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(FireElemental::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(LavaGolem::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Apple::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Ash::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Brine::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Celeste::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Cindra::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Drak::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Igneus::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Lucifer::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Magnus::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Pan::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(PrincessMittens::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Ragnis::getHexusOpponentData()));
	this->opponents.push_back(HexusOpponentPreview::create(Scaldor::getHexusOpponentData()));


	for (std::vector<HexusOpponentPreview*>::iterator it = this->opponents.begin(); it != this->opponents.end(); ++it)
	{
		this->scrollPane->addChild(*it);
	}
}

HexusOpponentMenuDaemonsHallow::~HexusOpponentMenuDaemonsHallow()
{
}
