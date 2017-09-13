#ifndef PHYSICSYSTEM_H
#define PHYSICSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../Physics/MotionState.h"

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

	void receive(entityx::ComponentAddedEvent<RigidBodyComponent>& entity)
	{
		std::cout << "HELLO" << std::endl;

		if (entity.entity.has_component<TransformComponent>())
			entity.component->Body.getMotionState()->SetTransformComponent(entity.entity.component<TransformComponent>().get());
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