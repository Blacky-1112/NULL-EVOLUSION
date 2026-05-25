#pragma once
#include <string>
#include <unordered_map>
#include "gameObject.h"

class Texture
{
private:
	static std::unordered_map<std::string, ID3D11ShaderResourceView*> m_TexturePool;
public:
	static ID3D11ShaderResourceView* Load(const char* FileName);
	static ID3D11ShaderResourceView* LoadDDS(const char* FileName);

};