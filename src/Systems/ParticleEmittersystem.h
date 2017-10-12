#ifndef PARTICLEEMITTERSYSTEM_H
#define PARTICLEEMITTERSYSTEM_H

#include <random>

#include <entityx/System.h>

#include "../Components/TransformComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/EmitterComponent.h"

#include "../Core/EntityFactory.h"

class ParticleEmitterSystem : public entityx::System<ParticleEmitterSystem>
{
public:
	ParticleEmitterSystem(EntityFactory *entityFactory) :
		mDistribution(-0.1, 0.1)
	{
		mEntityFactory = entityFactory;
	}

	~ParticleEmitterSystem() {}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		es.each<TransformComponent, EmitterComponent>([=](entityx::Entity entity, TransformComponent &transform, EmitterComponent &emitter)
		{
			emitter.EmitterSpawnTime += dt;

			if (emitter.EmitterSpawnTime > emitter.SpawnRate)
			{
				emitter.EmitterSpawnTime = 0;
				entityx::Entity particle = mEntityFactory->createFromDataFile(emitter.ParticleData, transform);

				if (particle.has_component<RigidBodyComponent>())
				{
					auto &rigidBodyComp = particle.component<RigidBodyComponent>();
					auto &particleComp = particle.component<ParticleComponent>();

					glm::vec3 velocity = glm::vec3(emitter.Velocity.x + mDistribution(mGenerator), emitter.Velocity.y + mDistribution(mGenerator), emitter.Velocity.z + mDistribution(mGenerator));

					rigidBodyComp->Body->ApplyImpulse(velocity, glm::vec3(0.0f, 0.0f, 0.0f));
				}
			}
		});
	}

	std::uniform_real_distribution<float> mDistribution;
	std::default_random_engine mGenerator;

	double mTime;
	EntityFactory *mEntityFactory;
};

#endif // PARTICLEEMITTERSYSTEM_H