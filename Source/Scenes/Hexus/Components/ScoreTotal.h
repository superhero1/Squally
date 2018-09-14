#pragma once
#include "cocos2d.h"

#include "Engine/Localization/Localization.h"
#include "Engine/Rendering/Components/MenuSprite.h"
#include "Engine/Utils/StrUtils.h"
#include "Resources.h"

#include "ComponentBase.h"

using namespace cocos2d;

class ScoreTotal : public ComponentBase
{
public:
	static ScoreTotal * create();

protected:
	void onStateChange(GameState* eventCustom) override;

private:
	ScoreTotal();
	~ScoreTotal();

	void initializePositions() override;
	void updateTotals(GameState* gameState);

	Sprite* playerTotalFrame;
	Label* playerTotal;
	Sprite* playerLeaderEmblem;

	Sprite* enemyTotalFrame;
	Label* enemyTotal;
	Sprite* enemyLeaderEmblem;
};