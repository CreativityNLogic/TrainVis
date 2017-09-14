#include "EntityFactory.h"

#include <fstream>
#include <vector>

#include "../Components/TagComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBody.h"

EntityFactory::EntityFactory(entityx::EntityManager &entityManager, RigidWorld *physicsWorld) :
	mEntityManager(entityManager),
	mPhysicsWorld(physicsWorld)
{

}

EntityFactory::~EntityFactory()
{

}

void EntityFactory::createFromLevelFile(const std::string &filename)
{
	nlohmann::json levelFile;
	std::ifstream input(filename);
	levelFile << input;

	createFromLevelFile(levelFile);
}

void EntityFactory::createFromLevelFile(nlohmann::json &levelFile)
{
	nlohmann::json entitiesData = levelFile["Entities"];
	for (nlohmann::json::iterator it = entitiesData.begin(); it != entitiesData.end(); ++it)
	{
		nlohmann::json entityData = entitiesData[it.key()];
		entityx::Entity entity = mEntityManager.create();

		if (!entityData["Transform"].is_null())
		{
			nlohmann::json transform = entityData["Transform"];
			loadTransformComponent(transform, *entity.assign<TransformComponent>());
		}

		nlohmann::json entityFile;
		std::string file = entityData["Data"];
		std::ifstream entDataFile(file);
		entityFile << entDataFile;

		loadComponents(entity, entityFile);
	}
}

entityx::Entity EntityFactory::createFromDataFile(const std::string &filename)
{
	nlohmann::json entityFile;
	std::ifstream input(filename);
	entityFile << input;

	entityx::Entity entity = mEntityManager.create();
	loadComponents(entity, entityFile);
	return entity;
}

entityx::Entity EntityFactory::createFromDataFile(const std::string &filename, TransformComponent comp)
{
	nlohmann::json entityFile;
	std::ifstream input(filename);
	entityFile << input;

	entityx::Entity entity = mEntityManager.create();
	entity.assign<TransformComponent>(comp);
	loadComponents(entity, entityFile);
	return entity;
}

void EntityFactory::loadComponents(entityx::Entity &entity, nlohmann::json entityFile)
{
	if (!entityFile["TagComponent"].is_null() && !entity.has_component<TagComponent>())
		loadTagComponent(entityFile["TagComponent"], *entity.assign<TagComponent>());

	if (!entityFile["TransformComponent"].is_null() && !entity.has_component<TransformComponent>())
		loadTransformComponent(entityFile["TransformComponent"], *entity.assign<TransformComponent>());

	if (!entityFile["GraphicsComponent"].is_null() && !entity.has_component<GraphicsComponent>())
		loadGraphicsComponent(entityFile["GraphicsComponent"], entity, *entity.assign<GraphicsComponent>());

	if (!entityFile["RigidBodyComponent"].is_null() && !entity.has_component<RigidBodyComponent>())
		loadRigidBodyComponent(entityFile["RigidBodyComponent"], entity, *entity.assign<RigidBodyComponent>());
}

// Tag Component
void EntityFactory::loadTagComponent(nlohmann::json component, TagComponent& tagComp)
{
	if (!component["Tag"].is_null())
	{
		nlohmann::json tag = component["Tag"];
		if (tag.is_string())
			tagComp.Tag = tag;
	}
}

// Transform Component  
void EntityFactory::loadTransformComponent(nlohmann::json trans, TransformComponent& comp)
{
	if (!trans["Position"].is_null())
	{
		nlohmann::json position = trans["Position"];
		if (position[0].is_number() && position[1].is_number() && position[2].is_number())
			comp.Position = { position[0], position[1], position[2] };
	}

	if (!trans["Rotation"].is_null())
	{
		nlohmann::json rotation = trans["Rotation"];
		if (rotation[0].is_number() && rotation[1].is_number() && rotation[2].is_number())
			comp.Rotation = glm::quat(glm::vec3(rotation[0], rotation[1], rotation[2]));
	}

	if (!trans["Scale"].is_null())
	{
		nlohmann::json scale = trans["Scale"];
		if (scale[0].is_number() && scale[1].is_number() && scale[2].is_number())
			comp.Scale = { scale[0], scale[1], scale[2] };
	}
}

// Graphics Component
void EntityFactory::loadGraphicsComponent(nlohmann::json component, entityx::Entity &entity, GraphicsComponent& graphics)
{
	if (!component["Filename"].is_null())
	{
		nlohmann::json filename = component["Filename"];
		if (filename.is_string())
			graphics.Model.LoadFromFile(filename);
	}
}

// RigidBody Component
void EntityFactory::loadRigidBodyComponent(nlohmann::json body, entityx::Entity &entity, RigidBodyComponent& rigidBody)
{
	btCollisionShape *shape = nullptr;

	if (!body["Shape"].is_null())
	{
		nlohmann::json shapeSection = body["Shape"];
		if (shapeSection["Type"] == "Box")
		{
			nlohmann::json boxExtent = shapeSection["Extents"];
			if (boxExtent[0].is_number() && boxExtent[1].is_number() && boxExtent[2].is_number())
				shape = new btBoxShape(btVector3(boxExtent[0], boxExtent[1], boxExtent[2]));
			else 
				shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		}
		else if (shapeSection["Type"] == "Circle")
		{
			float radius = 1.0f;
			if (shapeSection["Radius"].is_number())
				radius = shapeSection["Radius"];

			shape = new btSphereShape(radius);
		}
		else if (shapeSection["Type"] == "Convex")
		{
			std::string filename = shapeSection["Filename"];
			Model convexShape(filename);
		}
		else
		{
			shape = new btSphereShape(1.0f);
		}
	}

	//Get the inertia value.
	nlohmann::json inertiaData = body["Inertia"];
	glm::vec3 inertia;
	if (inertiaData[0].is_number() && inertiaData[1].is_number() && inertiaData[2].is_number())
		inertia = glm::vec3(inertiaData[0], inertiaData[1], inertiaData[2]);

	//Get the mass value.
	float mass = 0.0f;
	if (body["Mass"].is_number())
		mass = body["Mass"];

	nlohmann::json originData = body["Origin"];
	glm::vec3 origin;
	if (originData[0].is_number() && originData[1].is_number() && originData[2].is_number())
		origin = glm::vec3(originData[0], originData[1], originData[2]);

	btTransform transform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f));
	if (entity.has_component<TransformComponent>())
	{
		auto &transComp = entity.component<TransformComponent>();
		btVector3 position(transComp->Position.x, transComp->Position.y, transComp->Position.z);
		btQuaternion rotation(transComp->Rotation.x, transComp->Rotation.y, transComp->Rotation.z, transComp->Rotation.w);
		transform.setOrigin(position);
		transform.setRotation(rotation);
	}

	//Calculate the inertia.
	btVector3 localInertia(inertia.x, inertia.y, inertia.z);
	shape->calculateLocalInertia(mass, localInertia);

	btMotionState* motionState = new btDefaultMotionState(transform);

	RigidBody *newBody = new RigidBody(mass, motionState, shape, glm::vec3(localInertia.x(), localInertia.y(), localInertia.z()), origin);
	mPhysicsWorld->addRigidBody(newBody);
	rigidBody.Body = newBody;
}