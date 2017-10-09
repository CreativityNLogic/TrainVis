#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <vector>

class Cubemap
{
public:
	Cubemap();
	Cubemap(std::vector<std::string> textures_faces, bool gamma);

	unsigned int LoadFromFile(std::vector<std::string> textures_faces, bool gamma);
	int GetCubemapID() const;
	void Bind(unsigned int cubemapSlot);

private:
	std::vector<std::string> mTexture_faces;

	unsigned char *mData;
	unsigned int mCubemapID;
	unsigned int mID;
};

#endif 
