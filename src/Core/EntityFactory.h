#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <string>

#include <json/json.hpp>
#include <entityx/entityx.h>

class RenderWindow;

class RigidBody;
class RigidWorld;

struct TagComponent;
struct TransformComponent;
struct GraphicsComponent;
struct RigidBodyComponent;
struct LightComponent;
struct CameraComponent;
struct MaterialComponent;
struct SpriteComponent;
struct ParticleComponent;
struct EmitterComponent;

#include "../Graphics/Texture.h"
#include "../Components/FogComponent.h"

enum CollisionMasks
{
	NOCOLLISION = 0,
	STATIC,
	DYNAMIC,
	PLAYER
};

const int STATIC_DYNAMIC = DYNAMIC;
const int STATIC_DYNAMIC_PLAYER = DYNAMIC | PLAYER;
const int DYNAMIC_STATIC = STATIC;
const int DYNAMIC_STATIC_PLAYER = STATIC | PLAYER;
const int STATIC_STATIC = STATIC;
const int DYNAMIC_DYNAMIC = DYNAMIC;

class EntityFactory : public entityx::EntityX
{
public:
	EntityFactory(entityx::EntityManager &entityManager, RenderWindow *renderWindow, RigidWorld *physicsWorld);
	~EntityFactory();

	void createFromLevelFile(const std::string &filename);
	void createFromLevelFile(nlohmann::json &levelFile);
	entityx::Entity createFromDataFile(const std::string &filename);
	entityx::Entity createFromDataFile(const std::string &filename, TransformComponent comp);

	FogComponent &GetFog();

	bool IsDebug() const;

private:
	void loadComponents(entityx::Entity &entity, nlohmann::json entityFile);
	void loadTagComponent(nlohmann::json component, TagComponent& tagComp);
	void loadTransformComponent(nlohmann::json component, TransformComponent &animationComp);
	void loadGraphicsComponent(nlohmann::json component, entityx::Entity & entity, GraphicsComponent & graphics);
	void loadRigidBodyComponent(nlohmann::json body, entityx::Entity &entity, RigidBodyComponent& rigidBody);
	void loadLightComponent(nlohmann::json component, LightComponent& lightComp);
	void loadCameraComponent(nlohmann::json component, entityx::Entity &entity, CameraComponent& camComp);
	void loadMaterialComponent(nlohmann::json component, MaterialComponent& matComp);
	void loadSpriteComponent(nlohmann::json component, SpriteComponent& spriteComp);
	void loadParticleComponent(nlohmann::json component, ParticleComponent& particleComp);
	void loadEmitterComponent(nlohmann::json component, EmitterComponent& emitterComp);
	void loadFogComponent(nlohmann::json component);
	
	int getCollisionMask(const std::string &mask);
	int getCollisionGroup(const std::string &group);

private:
	entityx::EntityManager &mEntityManager;
	RigidWorld *mPhysicsWorld;
	RenderWindow *mRenderWindow;
	FogComponent mFog;
	Texture mTexture;
	bool mIsDebug;
};

#endif // ENTITYFACTORY_H