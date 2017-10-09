#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>

class Cubemap
{
public:
	Cubemap();
	Cubemap(const std::string &filename, bool gamma);

	void LoadFromFile(const std::string &filename, bool gamma);
	int GetCubemapID() const;
	void Bind(unsigned int cubemapSlot);

private:
	std::string mFilename;

	unsigned char *mData;
	unsigned int mCubemapID;
	unsigned int mID;
};

#endif 
