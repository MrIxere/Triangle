#pragma once
#include <string>

namespace gpr5300
{
	class CTexture
	{
	public:
		void CreateTexture(const std::string & file_path, int textureNumber);
		void Delete() const;
	private:
		unsigned char* data_{};
		unsigned int texture_ = 0;
		int texWidth_ = 0;
		int texHeight_ = 0;
		int nrChannels_ = 0;
	};
}