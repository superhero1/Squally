#include "HexEditingTab.h"

#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Config/ConfigManager.h"
#include "Engine/Events/NavigationEvents.h"
#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Save/SaveManager.h"
#include "Engine/UI/Controls/ScrollPane.h"
#include "Entities/Platformer/Helpers/BalmerPeaks/Snowman.h"
#include "Entities/Platformer/Helpers/EndianForest/Guano.h"
#include "Entities/Platformer/Special/Shopkeeper.h"
#include "Menus/Tutorials/TutorialEntry.h"
#include "Menus/Tutorials/TutorialSelectMenu.h"
#include "Scenes/Platformer/Level/PlatformerMap.h"
#include "Scenes/Platformer/Save/SaveKeys.h"
#include "Scenes/Tutorials/Save/TutorialSaveKeys.h"

#include "Resources/MapResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

HexEditingTab* HexEditingTab::create()
{
	HexEditingTab* instance = new HexEditingTab();

	instance->autorelease();

	return instance;
}

HexEditingTab::HexEditingTab()
{
	this->hexEditingGold = this->createEntry(TutorialSaveKeys::SaveKeyHexEditGold);

	this->shopkeeper = this->hexEditingGold->addEntity(Shopkeeper::deserialize(ValueMap()), Vec2(-24.0f, 32.0f));

	this->shopkeeper->setScale(0.65f);
}

HexEditingTab::~HexEditingTab()
{
}

void HexEditingTab::onEnter()
{
	super::onEnter();
}

void HexEditingTab::initializeListeners()
{
	super::initializeListeners();

	this->hexEditingGold->setClickCallback([=]()
	{
		this->loadHexEditingGoldTutorial();
	});
}

void HexEditingTab::initializePositions()
{
	super::initializePositions();
}

void HexEditingTab::loadHexEditingGoldTutorial()
{
	NavigationEvents::LoadScene(NavigationEvents::LoadSceneArgs([=]()
	{
		const int UNUSED_SAVE_PROFILE = 99;

		SaveManager::deleteAllProfileData(UNUSED_SAVE_PROFILE);
		SaveManager::setActiveSaveProfile(UNUSED_SAVE_PROFILE);
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWind, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeySpellBookWater, Value(true));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyHelperName, Value(Guano::MapKey));
		SaveManager::SoftSaveProfileData(SaveKeys::SaveKeyScrappyFound, Value(true));
		
		PlatformerMap* map = PlatformerMap::create(MapResources::Tutorials_KnownValueInt32);

		return map;
	}));
}
