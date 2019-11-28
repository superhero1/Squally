#include "LeafTerrain.h"

#include "cocos/base/CCValue.h"

#include "Resources/TerrainResources.h"
#include "Resources/TextureResources.h"

using namespace cocos2d;

const std::string LeafTerrain::MapKeyTerrainType = "leaf";

LeafTerrain* LeafTerrain::create(ValueMap& properties)
{
	LeafTerrain* instance = new LeafTerrain(properties);

	instance->autorelease();

	return instance;
}

LeafTerrain::LeafTerrain(ValueMap& properties) : super(
	properties,
	TerrainObject::TerrainData(
		0.5f,
		LeafTerrain::MapKeyTerrainType,
		TextureResources::LeafTexture,
		TerrainResources::Leaf_Top,
		TerrainResources::Leaf_TopLeft,
		TerrainResources::Leaf_TopRight,
		TerrainResources::Leaf_TopConnector,
		TerrainResources::BlueGrass_TopConnectorConcave,
		TerrainResources::BlueGrass_TopConnectorConcaveDeep,
		TerrainResources::BlueGrass_TopConnectorConvex,
		TerrainResources::BlueGrass_TopConnectorConvexDeep,
		TerrainResources::Leaf_Bottom,
		TerrainResources::Leaf_BottomLeft,
		TerrainResources::Leaf_BottomRight,
		TerrainResources::Leaf_BottomConnector,
		TerrainResources::Leaf_Left,
		TerrainResources::Leaf_LeftConnector,
		TerrainResources::Leaf_Right,
		TerrainResources::Leaf_RightConnector,
		// Top
		Vec2::ZERO,
		// Bottom
		Vec2::ZERO,
		// Left
		Vec2::ZERO,
		// Right
		Vec2::ZERO,
		// TopLeft
		Vec2::ZERO,
		// TopRight
		Vec2::ZERO,
		// BottomLeft
		Vec2::ZERO,
		// BottomRight
		Vec2::ZERO,
		// TopConnector
		Vec2::ZERO,
		// TopConnectorConcave
		Vec2::ZERO,
		// TopConnectorConcaveDeep
		Vec2::ZERO,
		// TopConnectorConvex
		Vec2::ZERO,
		// TopConnectorConvexDeep
		Vec2::ZERO,
		Color4B(11, 30, 39, 255)))
{
}

LeafTerrain::~LeafTerrain()
{
}