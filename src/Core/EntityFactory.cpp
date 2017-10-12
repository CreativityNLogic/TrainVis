#include "EntityFactory.h"

#include <fstream>
#include <vector>
#include <algorithm>

#include "../Components/TagComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/EmitterComponent.h"

#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBody.h"
#include "../Physics/MotionState.h"

#include "../Graphics/RenderWindow.h"
#include "../Graphics/Material.h"

EntityFactory::EntityFactory(entityx::EntityManager &entityManager, RenderWindow *renderWindow, RigidWorld *physicsWorld) :
	mEntityManager(entityManager),
	mRenderWindow(renderWindow),
	mPhysicsWorld(physicsWorld)
{
	mTexture.LoadFromFile("../../assets/textures/smoke2.png", false);
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

	if (!entityFile["LightComponent"].is_null() && !entity.has_component<LightComponent>())
		loadLightComponent(entityFile["LightComponent"], *entity.assign<LightComponent>());

	if (!entityFile["CameraComponent"].is_null() && !entity.has_component<CameraComponent>())
		loadCameraComponent(entityFile["CameraComponent"], entity, *entity.assign<CameraComponent>());

	if (!entityFile["MaterialComponent"].is_null() && !entity.has_component<MaterialComponent>())
		loadMaterialComponent(entityFile["MaterialComponent"], *entity.assign<MaterialComponent>());

	if (!entityFile["SpriteComponent"].is_null() && !entity.has_component<SpriteComponent>())
		loadSpriteComponent(entityFile["SpriteComponent"], *entity.assign<SpriteComponent>());

	if (!entityFile["ParticleComponent"].is_null() && !entity.has_component<ParticleComponent>())
		loadParticleComponent(entityFile["ParticleComponent"], *entity.assign<ParticleComponent>());

	if (!entityFile["EmitterComponent"].is_null() && !entity.has_component<EmitterComponent>())
		loadEmitterComponent(entityFile["EmitterComponent"], *entity.assign<EmitterComponent>());
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
		{
			glm::vec3 euler(rotation[0], rotation[1], rotation[2]);
			comp.Rotation = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z)));
		}
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

	auto &transComp = entity.component<TransformComponent>();
	btVector3 position(transComp->Position.x, transComp->Position.y, transComp->Position.z);
	btQuaternion rotation(transComp->Rotation.x, transComp->Rotation.y, transComp->Rotation.z, transComp->Rotation.w);
	transform.setOrigin(position);
	transform.setRotation(rotation);

	//Calculate the inertia.
	btVector3 localInertia(inertia.x, inertia.y, inertia.z);
	shape->calculateLocalInertia(mass, localInertia);

	btMotionState* motionState = new MotionState(transComp.get());

	RigidBody *newBody = new RigidBody(mass, motionState, shape, glm::vec3(localInertia.x(), localInertia.y(), localInertia.z()), origin);
	
	int group = -1;
	int mask = -1;

	if (body["Group"].is_string())
		group = getCollisionGroup(body["Group"]);

	if (body["Mask"].is_string())
		mask = getCollisionMask(body["Mask"]);

	if(group < 0 || mask < 0)
		mPhysicsWorld->addRigidBody(newBody);
	else
		mPhysicsWorld->addRigidBody(newBody, group, mask);

	rigidBody.Body = newBody;
}

// Light Component
void EntityFactory::loadLightComponent(nlohmann::json component, LightComponent& lightComp)
{
	if (component["Type"].is_string())
	{
		std::string type = component["Type"];
		if (type == "Directional")
			lightComp.LightType = LightComponent::DIRECTIONAL;
		else if (type == "Point")
			lightComp.LightType = LightComponent::POINT;
		else if (type == "Spot")
			lightComp.LightType = LightComponent::SPOTLIGHT;
		else
			lightComp.LightType = LightComponent::DIRECTIONAL;
	}

	if (component["Diffuse"].is_array())
		lightComp.Diffuse = glm::vec3(component["Diffuse"][0], component["Diffuse"][1], component["Diffuse"][2]);
	else
		lightComp.Diffuse = glm::vec3();

	if (component["Specular"].is_array())
		lightComp.Specular = glm::vec3(component["Specular"][0], component["Specular"][1], component["Specular"][2]);
	else
		lightComp.Specular = glm::vec3();

	if (component["Ambient"].is_array())
		lightComp.Ambient = glm::vec3(component["Ambient"][0], component["Ambient"][1], component["Ambient"][2]);
	else
		lightComp.Ambient = glm::vec3();

	if (component["AmbientStrength"].is_number_float())
		lightComp.AmbientStrength = component["AmbientStrength"];
	else
		lightComp.AmbientStrength = 1.0f;

	if (component["SpecularStrength"].is_number_float())
		lightComp.SpecularStrength = component["SpecularStrength"];
	else
		lightComp.SpecularStrength = 1.0f;

	if (component["Constant"].is_number_float())
		lightComp.Constant = component["Constant"];
	else
		lightComp.Constant = 1.0f;

	if (component["Linear"].is_number_float())
		lightComp.Linear = component["Linear"];
	else
		lightComp.Linear = 1.0f;

	if (component["Quadratic"].is_number_float())
		lightComp.Quadratic = component["Quadratic"];
	else
		lightComp.Quadratic = 1.0f;

	if (component["InnerCutoff"].is_number_float())
		lightComp.InnerCutoff = component["InnerCutoff"];
	else
		lightComp.InnerCutoff = 1.0f;

	if (component["OuterCutoff"].is_number_float())
		lightComp.OuterCutoff = component["OuterCutoff"];
	else
		lightComp.OuterCutoff = 1.0f;

}

// Camera Component
void EntityFactory::loadCameraComponent(nlohmann::json component, entityx::Entity &entity, CameraComponent& camComp)
{
	float speed = 1.0f;
	float sensitvity = 1.0f;
	float fov = 90.0f;
	float nearClip = 0.01f;
	float farClip = 1000.0f;

	if (component["Speed"].is_number_float())
		speed = component["Speed"];

	if (component["MouseSensitivity"].is_number_float())
		sensitvity = component["MouseSensitivity"];

	if (component["FOV"].is_number_float())
		fov = component["FOV"];

	if (component["NearClip"].is_number_float())
		nearClip = component["NearClip"];

	if (component["FarClip"].is_number_float())
		farClip = component["FarClip"];

	auto &transComp = entity.component<TransformComponent>();

	camComp.Camera.SetPosition(transComp->Position);
	camComp.Camera.SetCameraSpeed(speed);
	camComp.Camera.SetMouseSensitivity(sensitvity);
	camComp.Camera.SetProjection(fov, (float)mRenderWindow->GetWindowSize().x / (float)mRenderWindow->GetWindowSize().y, nearClip, farClip);
}

void EntityFactory::loadMaterialComponent(nlohmann::json component, MaterialComponent& matComp)
{
	for (nlohmann::json::iterator it = component.begin(); it != component.end(); ++it)
	{
		nlohmann::json materialData = component[it.key()];
		Material material;

		if (materialData["Shader"].is_array())
			material.loadShader(materialData["Shader"][0], materialData["Shader"][1]);

		if (materialData["Diffuse"].is_string())
			material.loadDiffuseTexture(materialData["Diffuse"]);

		if (materialData["Specular"].is_string())
			material.loadSpecularTexture(materialData["Specular"]);

		if (materialData["Normal"].is_string())
			material.loadNormalTexture(materialData["Normal"]);

		if (materialData["Emissive"].is_string())
			material.loadEmissiveTexture(materialData["Emissive"]);

		if (materialData["DiffuseColour"].is_array())
			material.setDiffuseColour(glm::vec4(materialData["DiffuseColour"][0], materialData["DiffuseColour"][1], materialData["DiffuseColour"][2], materialData["DiffuseColour"][3]));

		if (materialData["SpecularColour"].is_array())
			material.setSpecularColour(glm::vec4(materialData["SpecularColour"][0], materialData["SpecularColour"][1], materialData["SpecularColour"][2], materialData["SpecularColour"][3]));

		if (materialData["AmbientColour"].is_array())
			material.setAmbientColour(glm::vec4(materialData["AmbientColour"][0], materialData["AmbientColour"][1], materialData["AmbientColour"][2], materialData["AmbientColour"][3]));

		if (materialData["EmissiveColour"].is_array())
			material.setEmissiveColour(glm::vec4(materialData["EmissiveColour"][0], materialData["EmissiveColour"][1], materialData["EmissiveColour"][2], materialData["EmissiveColour"][3]));

		if (materialData["Shininess"].is_number_float())
			material.setShininess(materialData["Shininess"]);

		if (materialData["Transparent"].is_boolean())
			material.setIsTransparent(materialData["Transparent"]);

		matComp.Materials.push_back(material);
	}
}

void EntityFactory::loadSpriteComponent(nlohmann::json component, SpriteComponent& spriteComp)
{
	spriteComp.Sprite.LoadFromTexture(mTexture);

	//if (component["Filename"].is_string())
		//spriteComp.Sprite.LoadFromFile(component["Filename"]);

	if (component["IsBillboard"].is_boolean())
		spriteComp.Sprite.SetBillboard(component["IsBillboard"]);

	if (component["Opacity"].is_number_float())
		spriteComp.Sprite.SetOpacity(component["Opacity"]);
}

void EntityFactory::loadParticleComponent(nlohmann::json component, ParticleComponent& particleComp)
{
	if (component["EndTime"].is_number_float())
		particleComp.EndTime = component["EndTime"];
	if (component["FadeIn"].is_number_float())
		particleComp.FadeIn = component["FadeIn"];
	if (component["FadeOut"].is_number_float())
		particleComp.FadeOut = component["FadeOut"];
}

void EntityFactory::loadEmitterComponent(nlohmann::json component, EmitterComponent& emitterComp)
{
	if (component["ParticleData"].is_string())
		emitterComp.ParticleData = component["ParticleData"];
	if (component["Velocity"].is_array())
		emitterComp.Velocity = glm::vec3(component["Velocity"][0], component["Velocity"][1], component["Velocity"][2]);
	if (component["Offset"].is_array())
		emitterComp.Offset = glm::vec3(component["Offset"][0], component["Offset"][1], component["Offset"][2]);
	if (component["SpawnRate"].is_number_float())
		emitterComp.SpawnRate = component["SpawnRate"];
}

int EntityFactory::getCollisionMask(const std::string &mask)
{
	if (mask == "NOCOLLISION")
		return NOCOLLISION;
	else if (mask == "STATIC_DYNAMIC")
		return STATIC_DYNAMIC;
	else if (mask == "STATIC_DYNAMIC_PLAYER")
		return STATIC_DYNAMIC_PLAYER;
	else if (mask == "DYNAMIC_STATIC")
		return DYNAMIC_STATIC;
	else if (mask == "DYNAMIC_STATIC_PLAYER")
		return DYNAMIC_STATIC_PLAYER;
	else if (mask == "STATIC_STATIC")
		return STATIC_STATIC;
	else if (mask == "DYNAMIC_DYNAMIC")
		return DYNAMIC_DYNAMIC;
	else
		return NOCOLLISION;
}

int EntityFactory::getCollisionGroup(const std::string &group)
{
	if (group == "NOCOLLISION")
		return NOCOLLISION;
	else if (group == "STATIC")
		return STATIC;
	else if (group == "DYNAMIC")
		return DYNAMIC;
	else if (group == "PLAYER")
		return PLAYER;
	else
		return NOCOLLISION;
}