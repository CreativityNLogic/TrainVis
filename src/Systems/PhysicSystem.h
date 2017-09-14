#ifndef PHYSICSYSTEM_H
#define PHYSICSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class PhysicSystem : public entityx::System<PhysicSystem>
{
public:
	PhysicSystem() {}
	~PhysicSystem() {}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		es.each<TransformComponent, RigidBodyComponent>([dt](entityx::Entity entity, TransformComponent &transform, RigidBodyComponent &body)
		{
			btTransform trans;
			body.Body->getMotionState()->getWorldTransform(trans);

			glm::vec3 offset = body.Body->getOffset();

			transform.Position = glm::vec3(trans.getOrigin().getX() - offset.x, trans.getOrigin().getY() - offset.y, trans.getOrigin().getZ() - offset.z);
			transform.Rotation = glm::quat(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
		});
	};
};

#endif // PHYSICSYSTEM_H