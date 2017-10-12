#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <entityx/System.h>
#include "../Components/TagComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../Graphics/RenderWindow.h"

class MovementSystem : public entityx::System<MovementSystem>
{
public:
	MovementSystem() : mRenderWindow(nullptr) {}
	MovementSystem(RenderWindow *renderWindow) : mRenderWindow(renderWindow) {}
	~MovementSystem() {}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		if (mRenderWindow != nullptr)
		{
			es.each<TagComponent, RigidBodyComponent>([=](entityx::Entity entity, TagComponent &tag, RigidBodyComponent &body)
			{
				if (tag.Tag == "Train")
				{


					body.Body->Activate(true);

					if (mRenderWindow->IsKeyPressed(Key::UPARROW))
					{
						body.Body->ApplyCentralForce(glm::vec3(0.0f, 0.0f, 10000.0f));
					}
					else if (mRenderWindow->IsKeyPressed(Key::DOWNARROW))
					{
						body.Body->ApplyCentralForce(glm::vec3(0.0f, 0.0f, -10000.0f));
					}
				}
			});
		}

	};

private:
	RenderWindow *mRenderWindow;
};

#endif // MOVEMENTSYSTEM_H