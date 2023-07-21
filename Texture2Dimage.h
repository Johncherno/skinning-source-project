#ifndef TEXTURE2DIMAGE_H
#define  TEXTURE2DIMAGE_H
#include<string>
using std::string;
class texture2D
{
public:
	void loadtexture(const string& filename);
	void bindtexture(unsigned int unit);
	void unbindtexture();
private:
	unsigned int mtexture;
};
#endif 
