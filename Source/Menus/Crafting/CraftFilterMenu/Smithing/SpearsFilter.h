#pragma once

#include "Menus/Crafting/CraftFilterMenu/CraftFilterEntry.h"

class Item;
class LocalizedLabel;

class SpearsFilter : public CraftFilterEntry
{
public:
	static SpearsFilter* create();

	std::vector<Item*> filter(std::vector<Item*> itemList) override;

protected:
	SpearsFilter();
	virtual ~SpearsFilter();
	
	void onEnter() override;
	void initializePositions() override;

private:
	typedef CraftFilterEntry super;
};