#include <texture.h>
#include <gl/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gpr5300
{
	void CTexture::CreateTexture(const std::string& file_path, int textureNumber)
	{
		data_ = stbi_load(file_path.c_str(),&texWidth_, &texHeight_, &nrChannels_, 0);
		glActiveTexture(textureNumber);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth_, texHeight_,0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void CTexture::Delete() const
	{
		glDeleteTextures(1, &texture_);
	}
}