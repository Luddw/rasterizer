#include "texture.h"
#include <iostream>
#include <stb_image.h>


Texture::Texture(const std::string &path) :  tex_handle(0),filepath(path), localbuf(nullptr),widht(0),height(0),bpp(0)
{
	stbi_set_flip_vertically_on_load(true);

	localbuf = stbi_load(path.c_str(),&widht,&height,&bpp, 4);
	if (localbuf == nullptr)
		std::cout << "Texture file failed to load" << std::endl;

	
	glGenTextures(1,&tex_handle);
	glBindTexture(GL_TEXTURE_2D,tex_handle);
	//std::cout<<"width: "<< widht<< "height: " << height << "bpp: "<<bpp<<std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,widht,height,0,GL_RGBA,GL_UNSIGNED_BYTE,localbuf);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	
	if (localbuf) //clears img data
		stbi_image_free(localbuf);

}

Texture::Texture(unsigned char * buf) :  tex_handle(0), localbuf(nullptr),widht(0),height(0),bpp(0)
{
	stbi_set_flip_vertically_on_load(true);

	localbuf = buf;
	if (localbuf == nullptr)
		std::cout << "Texture file failed to load" << std::endl;

	
	glGenTextures(1,&tex_handle);
	glBindTexture(GL_TEXTURE_2D,tex_handle);
	//std::cout<<"width: "<< widht<< "height: " << height << "bpp: "<<bpp<<std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,widht,height,0,GL_RGBA,GL_UNSIGNED_BYTE,localbuf);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	
	if (localbuf) //clears img data
		stbi_image_free(localbuf);

}

Texture::Texture() :  tex_handle(0), localbuf(nullptr),widht(0),height(0),bpp(0)
{
	stbi_set_flip_vertically_on_load(true);
	localbuf = stbi_load(".. /texture1.png",&widht,&height,&bpp,4);
	glGenTextures(1,&tex_handle);
	glBindTexture(GL_TEXTURE_2D,tex_handle);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,widht,height,0,GL_RGBA,GL_UNSIGNED_BYTE,localbuf);
	glBindTexture(GL_TEXTURE_2D,0);
	
	if (localbuf) //clears img data
		stbi_image_free(localbuf);
	
	
}
Texture::~Texture()
{
	Unbind();
}
void Texture::Bind(unsigned int textslot) const
{
	glActiveTexture(GL_TEXTURE0+textslot);
	glBindTexture(GL_TEXTURE_2D,tex_handle);
}
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D,0);	
}

Pixel Texture::GetColor(vec3& uvCoord)
{
	// fetch position in the texture's buffer, offsetted by bits per pixel
	const unsigned int x =  fmax(fmin(uvCoord.x * widht, widht), 0);
	const unsigned int y = fmax(fmin(uvCoord.y * height, height), 0);
	const unsigned char* imageColor = &localbuf[(x + y * widht) * bpp];
	
	return {
		imageColor[0],
		imageColor[1],
		imageColor[2],
		bpp == 4 ? imageColor[4] : (unsigned char)0xFF
	};
}