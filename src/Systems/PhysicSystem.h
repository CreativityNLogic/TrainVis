#ifndef PHYSICSYSTEM_H
#define PHYSICSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class PhysicSystem : public entityx::System<PhysicSystem>, public entityx::Receiver<PhysicSystem>
{
public:
	PhysicSystem() {}
	~PhysicSystem() {}

	void configure(entityx::EventManager &event_manager) 
	{
		event_manager.subscribe<entityx::ComponentAddedEvent<RigidBodyComponent>>(*this);
		event_manager.subscribe<entityx::ComponentRemovedEvent<RigidBodyComponent>>(*this);
	}

	void receive(const entityx::ComponentAddedEvent<RigidBodyComponent>& entity)
	{
		if (entity.entity.has_component<TransformComponent>())
		{

		}
	}

	void receive(const entityx::ComponentRemovedEvent<RigidBodyComponent>& entity)
	{

	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		es.each<TransformComponent, RigidBodyComponent>([dt](entityx::Entity entity, TransformComponent &transform, RigidBodyComponent &body)
		{

		});
	};
};

#endif // PHYSICSYSTEM_H