#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include "../Graphics/Model.h"

struct GraphicsComponent
{
	GraphicsComponent() {}
	GraphicsComponent(const std::string &filename) : Model(filename) {}
	Model Model;
};

#endif // GRAPHICCOMPONENT_H