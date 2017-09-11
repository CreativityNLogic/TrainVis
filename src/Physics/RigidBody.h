#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

class RigidBody
{
public:
	RigidBody(float mass, btMotionState *motionState, btCollisionShape *shape, glm::vec3 localInertia);
	virtual ~RigidBody();

	glm::vec3 getOrigin() const;
	btMotionState* getMotionState();
	btRigidBody * getRigidBody();

private:
	btRigidBody* mRigidBody;
};

#endif // RIGIDBODY_H