#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture 
{
public:
	Texture();
	Texture(const std::string &filename, bool gamma);
	
	void LoadFromFile(const std::string &filename, bool gamma);
	int GetTextureID() const;
	void Bind(unsigned int textureSlot);

private:
	std::string mFilename;

	unsigned char *mData;
	unsigned int mTextureID;
	unsigned int mID;
};

#endif // TEXTURE_H
