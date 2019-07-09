#pragma once

#include "Engine/Terrain/TerrainDeserializer.h"

class FrostTerrainDeserializer : public TerrainDeserializer
{
public:
	static FrostTerrainDeserializer* create();

	static const std::string MapKeyTerrainTypeFrost;

private:
	typedef TerrainDeserializer super;
	
	FrostTerrainDeserializer();
	~FrostTerrainDeserializer();
};
