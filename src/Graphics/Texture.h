#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

using namespace std;

class Texture 
{
public:
	Texture();
	~Texture();
	Texture(const char *path, const string &directory, bool gamma);
	
	int GetTextureID() const;
	void Bind();

private:
	string mFilename;

	unsigned char *mData;
	unsigned int mTextureID;
	unsigned int mID;
};

#endif // TEXTURE_H
