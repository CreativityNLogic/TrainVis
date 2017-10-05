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

class EntityFactory : public entityx::EntityX
{
public:
	EntityFactory(entityx::EntityManager &entityManager, RenderWindow *renderWindow, RigidWorld *physicsWorld);
	~EntityFactory();

	void createFromLevelFile(const std::string &filename);
	void createFromLevelFile(nlohmann::json &levelFile);
	entityx::Entity createFromDataFile(const std::string &filename);
	entityx::Entity createFromDataFile(const std::string &filename, TransformComponent comp);

private:
	void loadComponents(entityx::Entity &entity, nlohmann::json entityFile);
	void loadTagComponent(nlohmann::json component, TagComponent& tagComp);
	void loadTransformComponent(nlohmann::json component, TransformComponent &animationComp);
	void loadGraphicsComponent(nlohmann::json component, entityx::Entity & entity, GraphicsComponent & graphics);
	void loadRigidBodyComponent(nlohmann::json body, entityx::Entity &entity, RigidBodyComponent& rigidBody);
	void loadLightComponent(nlohmann::json component, LightComponent& lightComp);
	void loadCameraComponent(nlohmann::json component, entityx::Entity &entity, CameraComponent& camComp);

private:
	entityx::EntityManager &mEntityManager;
	RigidWorld *mPhysicsWorld;
	RenderWindow *mRenderWindow;
};

#endif // ENTITYFACTORY_H