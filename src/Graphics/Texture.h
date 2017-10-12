#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

class Texture 
{
public:
	Texture();
	Texture(const Texture &texture);
	Texture(const std::string &filename, bool gamma);
	
	void LoadFromFile(const std::string &filename, bool gamma);
	void Bind(unsigned int textureSlot);

	void SetTextureID(unsigned int id);
	void SetFilename(const std::string &filename);

	int GetTextureID() const;
	std::string GetFilename() const;

private:
	std::string mFilename;
	unsigned int mTextureID;
};

#endif // TEXTURE_H
