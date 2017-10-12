#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <entityx/System.h>

#include "../Components/TransformComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

class ParticleSystem : public entityx::System<ParticleSystem>
{
public:
	ParticleSystem()
	{
	}

	~ParticleSystem() {}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		es.each<TransformComponent, SpriteComponent, ParticleComponent>([=](entityx::Entity entity, TransformComponent &transform, SpriteComponent &sprite, ParticleComponent &particle)
		{
			particle.Lifetime += dt;
			
			if (particle.FadeOut > 0.0f)
			{
				float fadeOutStart = particle.EndTime - particle.FadeOut;
				if (fadeOutStart >= 0.0f)
				{
					if (particle.Lifetime > fadeOutStart)
					{
						particle.FadeTime += dt;
						sprite.Sprite.SetOpacity(1.0f - (particle.FadeTime / particle.FadeOut));
					}
				}
			}

			if (particle.Lifetime > particle.EndTime)
			{
				entity.destroy();
			}
		});
	}
};

#endif // PARTICLESYSTEM_H