#include "main.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"

std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture::m_TexturePool;

ID3D11ShaderResourceView* Texture::Load(const char* FileName)
{
	if (m_TexturePool.count(FileName) > 0)
	{
		return m_TexturePool[FileName];
	}

	wchar_t wFileName[512];
	mbstowcs(wFileName, FileName, strlen(FileName) + 1);

	TexMetadata metadata;
	ScratchImage image;
	ScratchImage mipChain;
	ID3D11ShaderResourceView* texture;
	//画像読み込み
	LoadFromWICFile(wFileName, WIC_FLAGS_NONE, &metadata, image);

	//ミップマップ生成
	GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	//テクスチャ生成
	CreateShaderResourceView(Renderer::GetDevice(), mipChain.GetImages(),
		mipChain.GetImageCount(), mipChain.GetMetadata(), &texture);
	assert(texture);

	m_TexturePool[FileName] = texture;

	return texture;
}

ID3D11ShaderResourceView* Texture::LoadDDS(const char* FileName)
{
	if (m_TexturePool.count(FileName) > 0)
	{
		return m_TexturePool[FileName];
	}

	wchar_t wFileName[512];
	mbstowcs(wFileName, FileName, strlen(FileName) + 1);

	TexMetadata metadata;
	ScratchImage image;
	ID3D11ShaderResourceView* texture;
	//画像読み込み
	LoadFromDDSFile(wFileName, DDS_FLAGS_NONE, &metadata, image);

	//テクスチャ生成
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
		image.GetImageCount(), image.GetMetadata(), &texture);
	assert(texture);

	m_TexturePool[FileName] = texture;

	return texture;
}