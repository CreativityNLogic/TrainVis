#ifndef JOINTTRANSFORM_H
#define JOINTTRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class JointTransform
{
public:
	JointTransform()
	{

	}

	JointTransform(glm::vec3 position, glm::quat rotation) :
		mPosition(position),
		mRotation(rotation)
	{

	}

	glm::mat4 GetLocalTransform() const
	{
		glm::mat4 matrix(1.0f);
		matrix = glm::translate(matrix, mPosition);
		matrix = matrix * glm::mat4_cast(mRotation);
	}

	static JointTransform Interpolate(JointTransform frameA, JointTransform frameB, float progression)
	{
		return JointTransform(glm::mix(frameA.mPosition, frameB.mPosition, progression), glm::lerp(frameA.mRotation, frameB.mRotation, progression));
	}

private:
	glm::vec3 mPosition;
	glm::quat mRotation;
};

#endif // JOINTTRANSFORM_H