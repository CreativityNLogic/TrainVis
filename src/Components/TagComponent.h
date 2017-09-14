#ifndef TAGCOMPONENT_H
#define TAGCOMPONENT_H

#include <string>

struct TagComponent
{
	TagComponent() :
		Tag("")
	{

	}

	TagComponent(const std::string tag) :
		Tag(tag)
	{

	}

	std::string Tag;
};

#endif // TAGCOMPONENT_H