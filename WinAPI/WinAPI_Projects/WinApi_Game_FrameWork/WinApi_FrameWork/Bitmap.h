#pragma once

#include <Windows.h>
#include <string>

#pragma comment (lib, "Msimg32.lib")

class CBitmap
{
public:

	CBitmap();
	CBitmap(const std::string fileDir);
	CBitmap(const CBitmap& bitmap);

	~CBitmap();

public:

	bool LoadBitmapByPath(const std::string fileDir);

public:

	bool empty() const;

	HBITMAP getBitmapHandle() const;
	const BITMAP& getBitmapInfo() const;
	std::string getFileDirectory() const;

public:

	void drawBitmap(HDC hdc, int x, int y, int width, int height) const;
	void drawBitmapByCropping(HDC hdc, int x, int y, int width, int height, int sx, int sy, int sw, int sh) const;

protected:

	HBITMAP	m_BitmapHandle;
	BITMAP m_BitmapInfo;
	std::string m_FileDir;

public:

	static void setTransparentColor(DWORD color);
	static DWORD getTransparentColor();

private:

	static DWORD m_TransparentColor;

};
