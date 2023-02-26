#include "TextureMgr.h"

TextureMgr::TextureMgr() : md3dDevice(0)
{
}

TextureMgr::~TextureMgr()
{
	for(auto it = mTextureSRV.begin(); it != mTextureSRV.end(); ++it)
    {
		if (it->second) {
			it->second->Release();
			it->second = 0;
		}
    }

	mTextureSRV.clear();
}

void TextureMgr::Init(ID3D11Device* device)
{
	md3dDevice = device;
}

ID3D11ShaderResourceView* TextureMgr::CreateTexture(std::wstring filename)
{
	ID3D11ShaderResourceView* srv = 0;

	// Does it already exist?
	if( mTextureSRV.find(filename) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		std::wstring ext = L"";
		for (int i = filename.length() - 1; i >= 0; i--)
		{
			if (filename[i] == '.')
				break;
			ext.insert(ext.begin(), tolower(filename[i]));
		}

		if (ext == L"dds")
			HR(CreateDDSTextureFromFile(md3dDevice, filename.c_str(), NULL, &srv));
		
		if (ext != L"dds")
			HR(CreateWICTextureFromFile(md3dDevice, filename.c_str(), NULL, &srv));

		mTextureSRV[filename] = srv;
	}

	return srv;
}
