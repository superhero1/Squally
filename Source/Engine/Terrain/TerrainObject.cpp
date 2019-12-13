#include "TerrainObject.h"

#include <iomanip>
#include <sstream>

#include "cocos/2d/CCClippingNode.h"
#include "cocos/2d/CCNode.h"
#include "cocos/2d/CCLabel.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"
#include "cocos/base/CCValue.h"
#include "cocos/renderer/CCGLProgram.h"

#include "Engine/Camera/GameCamera.h"
#include "Engine/Config/ConfigManager.h"
#include "Engine/Events/TerrainEvents.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Physics/EngineCollisionTypes.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/LogUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Engine/Utils/RenderUtils.h"

#include "Resources/ShaderResources.h"

using namespace cocos2d;

std::string TerrainObject::MapKeyTypeTerrain = "terrain";
std::string TerrainObject::MapKeyTypeIsHollow = "is-hollow";
std::string TerrainObject::MapKeyTypeTopOnly = "top-only";
const float TerrainObject::ShadowDistance = 32.0f;
const float TerrainObject::InfillDistance = 128.0f;
const float TerrainObject::TopThreshold = float(M_PI) / 6.0f;
const float TerrainObject::BottomThreshold = 7.0f * float(M_PI) / 6.0f;

TerrainObject::TerrainObject(ValueMap& properties, TerrainData terrainData) : super(properties)
{
	this->terrainData = terrainData;
	this->points = std::vector<Vec2>();
	this->intersectionPoints = std::vector<Vec2>();
	this->segments = std::vector<std::tuple<Vec2, Vec2>>();
	this->collisionSegments = std::vector<std::tuple<Vec2, Vec2>>();
	this->textureTriangles = std::vector<AlgoUtils::Triangle>();
	this->infillTriangles = std::vector<AlgoUtils::Triangle>();
	this->isHollow = GameUtils::getKeyOrDefault(this->properties, TerrainObject::MapKeyTypeIsHollow, Value(false)).asBool();
	this->isTopOnlyCollision = GameUtils::getKeyOrDefault(this->properties, TerrainObject::MapKeyTypeTopOnly, Value(false)).asBool();
	this->isInactive = GameUtils::getKeyOrDefault(this->properties, CollisionObject::MapKeyTypeCollision, Value("")).asString() == CollisionObject::MapKeyCollisionTypeNone;
	this->isFlipped = GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyFlipY, Value(false)).asBool();

	this->collisionNode = Node::create();
	this->infillTexturesNode = Node::create();
	this->infillNode = Node::create();
	this->shadowsNode = Node::create();
	this->leftWallNode = Node::create();
	this->rightWallNode = Node::create();
	this->bottomsNode = Node::create();
	this->bottomCornersNode = Node::create();
	this->topsNode = Node::create();
	this->connectorsNode = Node::create();
	this->topCornersNode = Node::create();
	this->debugLevel2Node = Node::create();
	this->boundsRect = Rect::ZERO;

	this->debugLevel2Node->setVisible(false);

	this->addChild(this->collisionNode);
	this->addChild(this->infillTexturesNode);
	this->addChild(this->infillNode);
	this->addChild(this->shadowsNode);
	this->addChild(this->leftWallNode);
	this->addChild(this->rightWallNode);
	this->addChild(this->bottomsNode);
	this->addChild(this->topsNode);
	this->addChild(this->connectorsNode);
	this->addChild(this->bottomCornersNode);
	this->addChild(this->topCornersNode);
	this->addChild(this->debugLevel2Node);

	this->initResources();
	this->setPoints(this->polylinePoints);
	this->rebuildTerrain(terrainData);
}

TerrainObject::~TerrainObject()
{ 
}

void TerrainObject::onEnter()
{
	super::onEnter();

	// Should get called right after this is terrain is added to the map
	if (!this->isInactive)
	{
		TerrainEvents::TriggerResolveOverlapConflicts(TerrainEvents::TerrainOverlapArgs(this));
	}
}

void TerrainObject::onEnterTransitionDidFinish()
{
	super::onEnterTransitionDidFinish();
	
	// This gets built as a deferred step since we may be waiting on masking until this point
	this->buildCollision();
}

void TerrainObject::onDeveloperModeEnable(int debugLevel)
{
	if (debugLevel >= 2)
	{
		this->debugLevel2Node->setVisible(true);
	}
}

void TerrainObject::onDeveloperModeDisable()
{
	this->debugLevel2Node->setVisible(false);
}

void TerrainObject::initializeListeners()
{
	super::initializeListeners();
	
	if (!this->isHollow && !this->isInactive)
	{
		this->addEventListener(EventListenerCustom::create(TerrainEvents::EventResolveOverlapConflicts, [=](EventCustom* eventCustom)
		{
			TerrainEvents::TerrainOverlapArgs* args = static_cast<TerrainEvents::TerrainOverlapArgs*>(eventCustom->getUserData());

			if (args != nullptr && args->newTerrain != this)
			{
				this->maskAgainstOther(args->newTerrain);
			}
		}));
	}
}

void TerrainObject::initResources()
{
	if (!this->isFlipped)
	{
		return;
	}

	auto swapResources = [=](std::string* resourceA, Vec2* offsetA, std::string* resourceB, Vec2* offsetB)
	{
		std::string tempStr = *resourceA;
		Vec2 tempVec = *offsetA;

		*resourceA = *resourceB;
		*offsetA = *offsetB;
		*resourceB = tempStr;
		*offsetB = tempVec;

		offsetA->y *= -1.0f;
		offsetB->y *= -1.0f;
	};

	swapResources(
		&this->terrainData.topCornerRightResource, &this->terrainData.topRightCornerOffset,
		&this->terrainData.bottomCornerRightResource, &this->terrainData.bottomRightCornerOffset
	);

	swapResources(
		&this->terrainData.topCornerLeftResource, &this->terrainData.topLeftCornerOffset,
		&this->terrainData.bottomCornerLeftResource, &this->terrainData.bottomLeftCornerOffset
	);

	swapResources(
		&this->terrainData.topResource, &this->terrainData.topOffset,
		&this->terrainData.bottomResource, &this->terrainData.bottomOffset
	);

	swapResources(
		&this->terrainData.topConnectorResource, &this->terrainData.topConnectorOffset,
		&this->terrainData.bottomConnectorResource, &this->terrainData.bottomConnectorOffset
	);
}

void TerrainObject::setPoints(std::vector<Vec2> points)
{
	this->points = points;
	this->segments = AlgoUtils::buildSegmentsFromPoints(this->points);
	this->collisionSegments = this->segments;
	this->textureTriangles = AlgoUtils::trianglefyPolygon(this->points);
	this->infillTriangles = this->textureTriangles;
}

void TerrainObject::rebuildTerrain(TerrainData terrainData)
{
	this->debugLevel2Node->removeAllChildren();

	this->buildInnerTextures();

	switch(ConfigManager::getGraphics())
	{
		case ConfigManager::GraphicsSetting::SlowHighQuality:
		{
			// Zac: Disabled for now since this is pretty slow
			// this->buildInfill(terrainData.infillColor);
			// this->buildSurfaceShadow();
			break;
		}
		case ConfigManager::GraphicsSetting::FastLowQuality:
		{
			break;
		}
	}

	this->buildSurfaceTextures();
}

void TerrainObject::buildCollision()
{
	this->collisionNode->removeAllChildren();

	if (this->isInactive)
	{
		return;
	}
	
	this->removeHollowEdgeCollisions();

	// Clear x/y position -- this is already handled by this TerrainObject, and would otherwise result in incorrectly placed collision
	this->properties[GameObject::MapKeyXPosition] = 0.0f;
	this->properties[GameObject::MapKeyYPosition] = 0.0f;

	std::string deserializedCollisionName = GameUtils::getKeyOrDefault(this->properties, CollisionObject::MapKeyTypeCollision, Value("")).asString();
	std::tuple<Vec2, Vec2>* previousSegment = nullptr;
	
	for (auto it = this->collisionSegments.begin(); it != this->collisionSegments.end(); it++)
	{
		auto itClone = it;
		std::tuple<Vec2, Vec2> segment = *it;
		std::tuple<Vec2, Vec2> nextSegment = (++itClone) == this->collisionSegments.end() ? this->collisionSegments[0] : (*itClone);
		PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT;
		material.friction = MathUtils::clamp(this->terrainData.friction, 0.0f, 1.0f);
		PhysicsBody* physicsBody = PhysicsBody::createEdgeSegment(std::get<0>(*it), std::get<1>(*it), material, 2.0f);
		CollisionObject* collisionObject = nullptr;

		if (deserializedCollisionName != "")
		{
			collisionObject = CollisionObject::create(this->properties, physicsBody, deserializedCollisionName, false, false);
		}
		else
		{
			float normalAngle = AlgoUtils::getSegmentNormalAngle(*it, this->textureTriangles);
			
			if ((!this->isFlipped && this->isTopAngle(normalAngle)) || (this->isFlipped && this->isBottomAngle(normalAngle)))
			{
				collisionObject = CollisionObject::create(this->properties, physicsBody, (CollisionType)EngineCollisionTypes::PassThrough, false, false);
			}
			else if ((!this->isFlipped && this->isBottomAngle(normalAngle)) || (this->isFlipped && this->isTopAngle(normalAngle)))
			{
				if (this->isTopCollisionFriendly(previousSegment, &segment, &nextSegment))
				{
					collisionObject = CollisionObject::create(this->properties, physicsBody, (CollisionType)EngineCollisionTypes::SolidRoof, false, false);
				}
			}
			else
			{
				if (this->isTopCollisionFriendly(previousSegment, &segment, &nextSegment))
				{
					collisionObject = CollisionObject::create(this->properties, physicsBody, (CollisionType)EngineCollisionTypes::Solid, false, false);
				}
			}
		}
		
		if (collisionObject != nullptr)
		{
			this->collisionNode->addChild(collisionObject);
		}

		previousSegment = &(*it);
	}

	for (auto it = this->intersectionPoints.begin(); it != this->intersectionPoints.end(); it++)
	{
		const float Radius = 32.0f;

		PhysicsMaterial material = PHYSICSBODY_MATERIAL_DEFAULT;
		material.friction = MathUtils::clamp(this->terrainData.friction, 0.0f, 1.0f);
		PhysicsBody* physicsBody = PhysicsBody::createCircle(Radius, material, *it);
		CollisionObject* collisionObject = CollisionObject::create(this->properties, physicsBody, (CollisionType)EngineCollisionTypes::Intersection, false, false);

		this->collisionNode->addChild(collisionObject);
	}
}

void TerrainObject::buildInnerTextures()
{
	this->infillTexturesNode->removeAllChildren();

	if (this->textureTriangles.empty())
	{
		return;
	}

	DrawNode* stencil = DrawNode::create();

	for (auto it = this->textureTriangles.begin(); it != this->textureTriangles.end(); it++)
	{
		AlgoUtils::Triangle triangle = *it;

		stencil->drawTriangle(triangle.coords[0], triangle.coords[1], triangle.coords[2], Color4F::GREEN);
	}

	ClippingNode* clip = ClippingNode::create(stencil);

	// Create parameters to repeat the texture
	Texture2D::TexParams params = Texture2D::TexParams();
	params.minFilter = GL_LINEAR;
	params.magFilter = GL_LINEAR;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;

	Sprite* texture = Sprite::create(this->terrainData.textureResource);
	Rect drawRect = AlgoUtils::getPolygonRect(this->points);

	this->boundsRect = Rect(drawRect.origin + this->getPosition(), drawRect.size);

	texture->setAnchorPoint(Vec2(0.0f, 0.0f));
	texture->getTexture()->setTexParameters(params);
	texture->setPosition(drawRect.origin);
	texture->setTextureRect(Rect(0.0f, 0.0f, drawRect.size.width - drawRect.origin.x, drawRect.size.height - drawRect.origin.y));
	clip->addChild(texture);

	this->infillTexturesNode->addChild(clip);
}

void TerrainObject::buildInfill(Color4B infillColor)
{
	this->infillNode->removeAllChildren();

	if (this->textureTriangles.empty())
	{
		return;
	}

	std::vector<Vec2> infillPoints = AlgoUtils::insetPolygon(this->textureTriangles, this->segments, TerrainObject::InfillDistance);
	std::vector<AlgoUtils::Triangle> infillTriangles = AlgoUtils::trianglefyPolygon(infillPoints);

	DrawNode* infill = DrawNode::create();

	// Invisible padding up to the original triangle size
	for (auto it = this->textureTriangles.begin(); it != this->textureTriangles.end(); it++)
	{
		AlgoUtils::Triangle triangle = *it;

		infill->drawTriangle(triangle.coords[0], triangle.coords[1], triangle.coords[2], Color4F(Color3B(infillColor), 0.0f));
	}

	// Loop over all infill triangles and create the solid infill color
	for (auto it = infillTriangles.begin(); it != infillTriangles.end(); it++)
	{
		AlgoUtils::Triangle triangle = *it;

		infill->drawTriangle(triangle.coords[0], triangle.coords[1], triangle.coords[2], Color4F(infillColor));
	}

	// Render the infill to a texture (Note: using outer points, not the infill points, due to the earlier padding)
	Rect infillRect = AlgoUtils::getPolygonRect(this->points);

	Sprite* renderedInfill = RenderUtils::renderNodeToSprite(infill, infillRect.origin, infillRect.size);
	Sprite* rasterizedInfill = RenderUtils::applyShaderOnce(renderedInfill, ShaderResources::Vertex_Blur, ShaderResources::Fragment_Blur, [=](GLProgramState* state)
	{
		state->setUniformVec2("resolution", Vec2(infillRect.size.width, infillRect.size.height));
		state->setUniformFloat("blurRadius", 112.0f);
		state->setUniformFloat("sampleNum", 24.0f);
	});
	rasterizedInfill->setAnchorPoint(Vec2::ZERO);
	rasterizedInfill->setPosition(infillRect.origin);

	this->infillNode->addChild(rasterizedInfill);
}

void TerrainObject::buildSurfaceShadow()
{
	this->shadowsNode->removeAllChildren();

	if (this->textureTriangles.empty())
	{
		return;
	}

	std::vector<Vec2> shadowPoints = AlgoUtils::insetPolygon(this->textureTriangles, this->segments, TerrainObject::ShadowDistance);
	std::vector<AlgoUtils::Triangle> shadowTriangles = AlgoUtils::trianglefyPolygon(shadowPoints);
	std::vector<std::tuple<Vec2, Vec2>> shadowSegments = AlgoUtils::buildSegmentsFromPoints(shadowPoints);

	DrawNode* shadowLine = DrawNode::create(12.0f);

	// Draw shadow lines below the floors
	for (auto it = shadowSegments.begin(); it != shadowSegments.end(); it++)
	{
		std::tuple<Vec2, Vec2> segment = *it;
		Vec2 source = std::get<0>(segment);
		Vec2 dest = std::get<1>(segment);
		Vec2 delta = dest - source;
		Vec2 midPoint = source.getMidpoint(dest);
		float segmentLength = source.distance(dest);
		float normalAngle = AlgoUtils::getSegmentNormalAngle(segment, shadowTriangles);

		if (normalAngle >= TerrainObject::TopThreshold && normalAngle <= float(M_PI) - TerrainObject::TopThreshold)
		{
			shadowLine->drawLine(source, dest, Color4F::BLACK);
		}
	}

	// Render the infill to a texture (Note: using outer points for padding)
	Rect shadowRect = AlgoUtils::getPolygonRect(this->points);

	Sprite* renderedShadowLine = RenderUtils::renderNodeToSprite(shadowLine, shadowRect.origin, shadowRect.size);

	if (renderedShadowLine != nullptr)
	{
		Sprite* rasterizedShadowLine = RenderUtils::applyShaderOnce(renderedShadowLine, ShaderResources::Vertex_Blur, ShaderResources::Fragment_Blur, [=](GLProgramState* state)
		{
			state->setUniformVec2("resolution", Vec2(shadowRect.size.width, shadowRect.size.height));
			state->setUniformFloat("blurRadius", 32.0f);
			state->setUniformFloat("sampleNum", 12.0f);
		});

		if (rasterizedShadowLine != nullptr)
		{
			rasterizedShadowLine->setAnchorPoint(Vec2::ZERO);
			rasterizedShadowLine->setPosition(shadowRect.origin);

			this->shadowsNode->addChild(rasterizedShadowLine);
		}
	}
}

void TerrainObject::buildSurfaceTextures()
{
	this->leftWallNode->removeAllChildren();
	this->rightWallNode->removeAllChildren();
	this->bottomsNode->removeAllChildren();
	this->bottomCornersNode->removeAllChildren();
	this->topsNode->removeAllChildren();
	this->topCornersNode->removeAllChildren();

	std::tuple<Vec2, Vec2>* previousSegment = nullptr;

	for (auto it = this->segments.begin(); it != this->segments.end(); it++)
	{
		auto itClone = it;

		std::tuple<Vec2, Vec2> segment = *it;
		std::tuple<Vec2, Vec2> nextSegment = (++itClone) == this->segments.end() ? this->segments[0] : (*itClone);
		Vec2 source = std::get<0>(segment);
		Vec2 dest = std::get<1>(segment);
		Vec2 delta = dest - source;
		Vec2 midPoint = source.getMidpoint(dest);
		float segmentLength = source.distance(dest);
		float angle = AlgoUtils::getSegmentAngle(segment, this->textureTriangles, this->debugLevel2Node);
		float normalAngle = AlgoUtils::getSegmentNormalAngle(segment, this->textureTriangles);
		float nextAngle = AlgoUtils::getSegmentAngle(nextSegment, this->textureTriangles);
		float nextSegmentNormalAngle = AlgoUtils::getSegmentNormalAngle(nextSegment, this->textureTriangles);
		float bisectingAngle = (nextAngle + angle) / 2.0f;
		float angleDelta = nextAngle - angle;

		// Figure out what the transition is between this segment and the next
		bool floorToWall = this->isTopAngle(normalAngle) && !this->isTopAngle(nextSegmentNormalAngle);
		bool wallToFloor = !this->isTopAngle(normalAngle) && this->isTopAngle(nextSegmentNormalAngle);
		bool roofToWall = this->isBottomAngle(normalAngle) && !this->isBottomAngle(nextSegmentNormalAngle);
		bool wallToRoof = !this->isBottomAngle(normalAngle) && this->isBottomAngle(nextSegmentNormalAngle);
		bool floorToFloor = this->isTopAngle(normalAngle) && this->isTopAngle(nextSegmentNormalAngle);
		bool leftToLeft = this->isLeftAngle(normalAngle) && this->isLeftAngle(nextSegmentNormalAngle);
		bool rightToRight = this->isRightAngle(normalAngle) && this->isRightAngle(nextSegmentNormalAngle);
		bool roofToRoof = this->isBottomAngle(normalAngle) && this->isBottomAngle(nextSegmentNormalAngle);

		std::stringstream angleStream;
		angleStream << std::fixed << std::setprecision(2) << (angle * 180.0f / float(M_PI));
		ConstantString* angleString = ConstantString::create(angleStream.str());

		std::stringstream bisectingAngleStream;
		bisectingAngleStream << std::fixed << std::setprecision(2) << (bisectingAngle * 180.0f / float(M_PI));
		ConstantString* bisectingAngleString = ConstantString::create(bisectingAngleStream.str());

		LocalizedLabel* angleDebug = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, angleString);
		LocalizedLabel* bisectingAngleDebug = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, bisectingAngleString);

		angleDebug->setTextColor(Color4B::YELLOW);
		bisectingAngleDebug->setTextColor(Color4B::MAGENTA);

		angleDebug->setPosition(midPoint + Vec2(0.0f, 24.0f));
		bisectingAngleDebug->setPosition(dest + Vec2(0.0f, 24.0f));

		this->debugLevel2Node->addChild(angleDebug);
		this->debugLevel2Node->addChild(bisectingAngleDebug);

		if (this->isTopAngle(normalAngle))
		{
			Sprite* top = Sprite::create(this->terrainData.topResource);

			top->setFlippedY(this->isFlipped);

			this->buildSegment(this->topsNode, top, Vec2(0.5f, 0.5f), source.getMidpoint(dest) + terrainData.topOffset, 180.0f - angle * 180.0f / float(M_PI), segmentLength, TileMethod::Horizontal);
		}
		else if (this->isBottomAngle(normalAngle))
		{
			Sprite* bottom = Sprite::create(this->terrainData.bottomResource);

			bottom->setFlippedY(this->isFlipped);

			this->buildSegment(this->bottomsNode, bottom, Vec2(0.5f, 0.5f), source.getMidpoint(dest) + terrainData.bottomOffset, 360.0f - angle * 180.0f / float(M_PI), segmentLength, TileMethod::Horizontal);
		}
		else if (this->isLeftAngle(normalAngle))
		{
			Sprite* left = Sprite::create(this->terrainData.leftResource);

			this->buildSegment(this->leftWallNode, left, Vec2(0.0f, 0.5f), source.getMidpoint(dest) + Vec2(0.0f, 0.0f), 270.0f - angle * 180.0f / float(M_PI), segmentLength, TileMethod::Vertical);
		}
		else
		{
			Sprite* right = Sprite::create(this->terrainData.rightResource);

			this->buildSegment(this->rightWallNode, right, Vec2(1.0f, 0.5f), source.getMidpoint(dest) + Vec2(0.0f, 0.0f), 90.0f - angle * 180.0f / float(M_PI), segmentLength, TileMethod::Vertical);
		}

		enum Concavity
		{
			Standard,
			ConvexMedium,
			ConvexDeep,
			ConcaveMedium,
			ConcaveDeep,
		};

		Concavity concavity = Concavity::Standard;
		float normalDeg = normalAngle * 180.0f / float(M_PI);
		float nextNormalDeg = nextSegmentNormalAngle * 180.0f / float(M_PI); 

		if (nextNormalDeg > normalDeg)
		{
			// Concave
			float normalDelta = nextNormalDeg - normalDeg;

			if (normalDelta <= 24.0f)
			{
				concavity = Concavity::Standard;
			}
			else if (normalDelta <= 96.0f)
			{
				concavity = Concavity::ConcaveMedium;
			}
			else
			{
				concavity = Concavity::ConcaveDeep;
			}
		}
		else
		{
			// Convex
			float normalDelta = normalDeg - nextNormalDeg;

			if (normalDelta <= 24.0f)
			{
				concavity = Concavity::Standard;
			}
			else if (normalDelta <= 96.0f)
			{
				concavity = Concavity::ConvexMedium;
			}
			else
			{
				concavity = Concavity::ConvexDeep;
			}
		}

		if (floorToFloor)
		{
			Sprite* topConnector = nullptr;
			Vec2 offset = terrainData.topOffset;
			
			ConstantString* str = ConstantString::create("S");
			LocalizedLabel* concavityLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::P, str);
			concavityLabel->setTextColor(Color4B::MAGENTA);
			concavityLabel->setPosition(dest + Vec2(0.0f, -24.0f));
			this->debugLevel2Node->addChild(concavityLabel);
			
			switch (concavity)
			{
				default:
				case Concavity::Standard:
				{
					topConnector = Sprite::create(this->terrainData.topConnectorResource);
					offset += this->terrainData.topOffset + this->terrainData.topConnectorOffset;
					str->setString("S");
					break;
				}
				case Concavity::ConvexMedium:
				{
					topConnector = Sprite::create(this->terrainData.topConnectorConvexResource);
					offset += this->terrainData.topOffset + this->terrainData.topConnectorConvexOffset;
					str->setString("t");
					break;
				}
				case Concavity::ConvexDeep:
				{
					topConnector = Sprite::create(this->terrainData.topConnectorConvexDeepResource);
					offset += this->terrainData.topOffset + this->terrainData.topConnectorConvexDeepOffset;
					str->setString("_T_");
					break;
				}
				case Concavity::ConcaveMedium:
				{
					topConnector = Sprite::create(this->terrainData.topConnectorConcaveResource);
					offset += this->terrainData.topOffset + this->terrainData.topConnectorConcaveOffset;
					str->setString("v");
					break;
				}
				case Concavity::ConcaveDeep:
				{
					topConnector = Sprite::create(this->terrainData.topConnectorConcaveDeepResource);
					offset += this->terrainData.topOffset + this->terrainData.topConnectorConcaveDeepOffset;
					str->setString("_V_");
					break;
				}
			}

			this->buildSegment(this->connectorsNode, topConnector, Vec2(0.5f, 0.5f), dest + offset, 180.0f - bisectingAngle * 180.0f / float(M_PI), segmentLength, TileMethod::None);
		}
		else if (roofToRoof)
		{
			Sprite* bottom = Sprite::create(this->terrainData.bottomConnectorResource);
			Vec2 offset = this->terrainData.bottomOffset + this->terrainData.bottomConnectorOffset;

			bottom->setFlippedY(this->isFlipped);

			this->buildSegment(this->connectorsNode, bottom, Vec2(0.5f, 0.5f), dest + offset, 360.0f - bisectingAngle * 180.0f / float(M_PI), segmentLength, TileMethod::None);
		}

		// Handle case when going from floor to walls
		if (floorToWall || wallToFloor)
		{
			Vec2 nextSource = std::get<0>(nextSegment);
			Vec2 nextDest = std::get<1>(nextSegment);

			bool isTopLeft = (floorToWall && nextSource.x <= source.x) || (wallToFloor && nextDest.x >= dest.x);

			if (isTopLeft)
			{
				Sprite* topLeft = Sprite::create(this->terrainData.topCornerLeftResource);
				Vec2 offset = this->terrainData.topOffset + this->terrainData.topLeftCornerOffset;

				topLeft->setFlippedY(this->isFlipped);

				this->buildSegment(this->topCornersNode, topLeft, Vec2(0.5f, 0.5f), dest + offset, 180.0f - (floorToWall ? angle : nextAngle) * 180.0f / float(M_PI), segmentLength, TileMethod::None);
			}
			else
			{
				Sprite* topRight = Sprite::create(this->terrainData.topCornerRightResource);
				Vec2 offset = this->terrainData.topOffset + this->terrainData.topRightCornerOffset;

				topRight->setFlippedY(this->isFlipped);

				this->buildSegment(this->topCornersNode, topRight, Vec2(0.5f, 0.5f), dest + offset, 180.0f - (floorToWall ? angle : nextAngle) * 180.0f / float(M_PI), segmentLength, TileMethod::None);
			}
		}
		// Handle case when going from roof to walls
		else if (roofToWall || wallToRoof)
		{
			Vec2 nextSource = std::get<0>(nextSegment);
			Vec2 nextDest = std::get<1>(nextSegment);

			bool isBottomLeft = ((roofToWall && nextSource.x <= source.x) || (wallToRoof && nextDest.x >= dest.x));

			if (isBottomLeft)
			{
				Sprite* bottomLeft = Sprite::create(this->terrainData.bottomCornerLeftResource);

				bottomLeft->setFlippedY(this->isFlipped);

				this->buildSegment(this->topCornersNode, bottomLeft, Vec2(0.0f, 0.5f), dest + this->terrainData.bottomLeftCornerOffset, 360.0f - (roofToWall ? angle : nextAngle) * 180.0f / float(M_PI), segmentLength, TileMethod::None);
			}
			else
			{
				Sprite* bottomRight = Sprite::create(this->terrainData.bottomCornerRightResource);

				bottomRight->setFlippedY(this->isFlipped);

				this->buildSegment(this->topCornersNode, bottomRight, Vec2(1.0f, 0.5f), dest + this->terrainData.bottomLeftCornerOffset, 360.0f - (roofToWall ? angle : nextAngle) * 180.0f / float(M_PI), segmentLength, TileMethod::None);
			}
		}

		previousSegment = &segment;
	}
}


void TerrainObject::buildSegment(Node* parent, Sprite* sprite, Vec2 anchor, Vec2 position, float rotation, float segmentLength, TerrainObject::TileMethod tileMethod)
{
	Size textureSize = sprite->getContentSize();

	static float seamlessSegmentX = 0.0f;

	// Create parameters to repeat the texture
	Texture2D::TexParams params = Texture2D::TexParams();
	params.minFilter = GL_LINEAR;
	params.magFilter = GL_LINEAR;

	switch(tileMethod)
	{
		case TileMethod::Horizontal:
		{
			params.wrapS = GL_REPEAT;
			break;
		}
		case TileMethod::Vertical:
		{
			params.wrapT = GL_REPEAT;
			break;
		}
		default:
		case TileMethod::None:
		{
			break;
		}
	}

	// Prevent off-by-1 rendering errors where texture pixels are barely separated
	segmentLength = std::ceil(segmentLength);

	sprite->setAnchorPoint(anchor);
	sprite->getTexture()->setTexParameters(params);

	// Start the texture from where the previous texture left off for seamless integration
	if (tileMethod == TileMethod::Horizontal)
	{
		sprite->setTextureRect(Rect(seamlessSegmentX, 0.0f, segmentLength, textureSize.height));

		// Advance the seamless segment distance (with wrap around on overflow)
		seamlessSegmentX = MathUtils::wrappingNormalize(seamlessSegmentX + segmentLength, 0.0f, textureSize.width);
	}
	else if (tileMethod == TileMethod::Vertical)
	{
		sprite->setTextureRect(Rect(0.0f, seamlessSegmentX, textureSize.width, segmentLength));

		// Advance the seamless segment distance (with wrap around on overflow)
		seamlessSegmentX = MathUtils::wrappingNormalize(seamlessSegmentX + segmentLength, 0.0f,  textureSize.height);
	}

	sprite->setPosition(position);
	sprite->setRotation(rotation);

	parent->addChild(sprite);
};

void TerrainObject::removeHollowEdgeCollisions()
{
	if (!this->isHollow || this->isInactive)
	{
		return;
	}
	
	this->collisionSegments.erase(std::remove_if(this->collisionSegments.begin(), this->collisionSegments.end(),
		[=](const std::tuple<cocos2d::Vec2, cocos2d::Vec2>& segment)
		{
			float normalAngle = AlgoUtils::getSegmentNormalAngle(segment, this->textureTriangles);
			
			// Remove all collision except for top collision
			return (!this->isTopAngle(normalAngle));
		}));
}

void TerrainObject::maskAgainstOther(TerrainObject* other)
{
	if (this->isInactive || !this->boundsRect.intersectsRect(other->boundsRect))
	{
		return;
	}

	// Remove all collision boxes that are completely eclipsed
	this->collisionSegments.erase(std::remove_if(this->collisionSegments.begin(), this->collisionSegments.end(),
		[=](const std::tuple<cocos2d::Vec2, cocos2d::Vec2>& segment)
		{
			int index = (&segment - &*this->collisionSegments.begin());

			Vec2 pointA = std::get<0>(segment);
			Vec2 pointB = std::get<1>(segment);
			bool isEclipsed[2] = { false, false };

			pointA += this->getPosition();
			pointB += this->getPosition();

			for (auto it = other->textureTriangles.begin(); it != other->textureTriangles.end(); it++)
			{
				AlgoUtils::Triangle triangle = *it;

				triangle.coords[0] += other->getPosition();
				triangle.coords[1] += other->getPosition();
				triangle.coords[2] += other->getPosition();

				isEclipsed[0] |= AlgoUtils::isPointInTriangle(triangle, pointA);
				isEclipsed[1] |= AlgoUtils::isPointInTriangle(triangle, pointB);

				if (isEclipsed[0] && isEclipsed[1])
				{
					break;
				}
			}

			if (isEclipsed[0] && isEclipsed[1])
			{
				return true;
			}
			else if (isEclipsed[0] || isEclipsed[1])
			{
				Vec2 eclipsedPoint = isEclipsed[0] ? pointA : pointB;
				Vec2 anchorPoint = isEclipsed[0] ? pointB : pointA;
				std::tuple<Vec2, Vec2> eclipsedSegment = std::tuple<Vec2, Vec2>(eclipsedPoint, anchorPoint);

				for (auto segmentIt = other->segments.begin(); segmentIt != other->segments.end(); segmentIt++)
				{
					cocos2d::Vec2 p1 = std::get<0>(*segmentIt);
					cocos2d::Vec2 p2 = std::get<1>(*segmentIt);

					p1 += other->getPosition();
					p2 += other->getPosition();
					
					std::tuple<Vec2, Vec2> candidateSegment = std::tuple<Vec2, Vec2>(p1, p2);

					if (AlgoUtils::doSegmentsIntersect(eclipsedSegment, candidateSegment))
					{
						Vec2 intersectionPoint = AlgoUtils::getLineIntersectionPoint(eclipsedSegment, candidateSegment) - this->getPosition();

						if (isEclipsed[0])
						{
							std::get<0>(this->collisionSegments[index]) = intersectionPoint;	
						}
						else
						{
							std::get<1>(this->collisionSegments[index]) = intersectionPoint;	
						}

						this->intersectionPoints.push_back(intersectionPoint);
					}
				}
			}

			return false;
		}), this->collisionSegments.end()
	);
}

bool TerrainObject::isTopAngle(float normalAngle)
{
	return normalAngle >= TerrainObject::TopThreshold && normalAngle <= float(M_PI) - TerrainObject::TopThreshold;
}

bool TerrainObject::isBottomAngle(float normalAngle)
{
	return normalAngle >= TerrainObject::BottomThreshold && normalAngle <= 2.0f * float(M_PI) - (TerrainObject::BottomThreshold - float(M_PI));
}

bool TerrainObject::isLeftAngle(float normalAngle)
{
	return (!this->isTopAngle(normalAngle) && !this->isBottomAngle(normalAngle) && normalAngle <= TerrainObject::BottomThreshold);
}

bool TerrainObject::isRightAngle(float normalAngle)
{
	return (!this->isTopAngle(normalAngle) && !this->isBottomAngle(normalAngle) && !this->isLeftAngle(normalAngle));
}

bool TerrainObject::isTopCollisionFriendly(std::tuple<Vec2, Vec2>* previousSegment, std::tuple<Vec2, Vec2>* segment, std::tuple<Vec2, Vec2>* nextSegment)
{
	if (!this->isTopOnlyCollision)
	{
		return true;
	}

	if (previousSegment == nullptr || segment == nullptr || nextSegment == nullptr)
	{
		return false;
	}

	float normalAngle = AlgoUtils::getSegmentNormalAngle(*segment, this->textureTriangles);
	float prevNormalAngle = AlgoUtils::getSegmentNormalAngle(*previousSegment, this->textureTriangles);
	float nextNormalAngle = AlgoUtils::getSegmentNormalAngle(*nextSegment, this->textureTriangles);
	bool isWallCollision = (this->isLeftAngle(normalAngle) || this->isRightAngle(normalAngle));

	if (isWallCollision && this->isTopAngle(prevNormalAngle) && this->isTopAngle(nextNormalAngle))
	{
		return true;
	}

	return false;
}
