#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <entityx/System.h>
#include "../Components/TagComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../Physics/RigidWorld.h"

class PhysicsSystem : public entityx::System<PhysicsSystem>, public entityx::Receiver<PhysicsSystem>
{
public:
	PhysicsSystem() : mRigidWorld(nullptr) {}
	PhysicsSystem(RigidWorld *rigidWorld) : mRigidWorld(rigidWorld) {}
	~PhysicsSystem() {}

	void configure(entityx::EventManager &event_manager)
	{
		event_manager.subscribe<entityx::ComponentRemovedEvent<RigidBodyComponent>>(*this);
	}

	void receive(const entityx::ComponentRemovedEvent<RigidBodyComponent> &comp)
	{
		auto &rigidBody = comp.component;
		mRigidWorld->removeRigidBody(rigidBody->Body);
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
	}

private:
	RigidWorld *mRigidWorld;
};

#endif // PHYSICSSYSTEM_H