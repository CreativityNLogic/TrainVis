#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

class RigidBody
{
public:
	RigidBody();
	RigidBody(float mass, btMotionState *motionState, btCollisionShape *shape, glm::vec3 localInertia, glm::vec3 offset = glm::vec3());
	~RigidBody();

	void setOffset(const glm::vec3 &offset);
	glm::vec3 getOffset() const;
	btMotionState* getMotionState();
	btRigidBody * getRigidBody();

	void ApplyCentralForce(const glm::vec3 &force);
	void ApplyCentralImpulse(const glm::vec3 &impulse);

	void ApplyForce(const glm::vec3 &force, const glm::vec3 &relativePos);
	void ApplyImpulse(const glm::vec3 &impulse, const glm::vec3 &relativePos);

private:
	btRigidBody* mRigidBody;
	glm::vec3 mOffset;
};

#endif // RIGIDBODY_H