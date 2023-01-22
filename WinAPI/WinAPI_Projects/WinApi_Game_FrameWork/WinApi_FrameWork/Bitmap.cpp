#include "BitMap.h"

DWORD CBitmap::m_TransparentColor = RGB(255, 0, 255);

CBitmap::CBitmap() : m_BitmapHandle(nullptr), m_FileDir("")
{
	memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
}

CBitmap::CBitmap(const CBitmap& bitmap) : CBitmap()
{
	this->LoadBitmapByPath(bitmap.getFileDirectory());
}

CBitmap::CBitmap(const std::string fileDir) : CBitmap()
{
	this->LoadBitmapByPath(fileDir);
}

CBitmap::~CBitmap()
{
	if (!this->empty())
		DeleteObject(m_BitmapHandle);
}

bool CBitmap::empty() const
{
	return m_BitmapHandle == nullptr;
}

bool CBitmap::LoadBitmapByPath(const std::string fileDir)
{
	m_BitmapHandle = (HBITMAP)LoadImageA(nullptr, fileDir.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (this->empty())
		return false;

	GetObject(m_BitmapHandle, sizeof(m_BitmapInfo), &m_BitmapInfo);
	m_FileDir = fileDir;

	return true;
}

HBITMAP CBitmap::getBitmapHandle() const
{
	return m_BitmapHandle;
}

const BITMAP& CBitmap::getBitmapInfo() const
{
	return m_BitmapInfo;
}

std::string CBitmap::getFileDirectory() const
{
	return m_FileDir;
}

void CBitmap::drawBitmap(HDC hdc, int x, int y, int width, int height) const
{
	this->drawBitmapByCropping(hdc, x, y, width, height, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight);
}

void CBitmap::drawBitmapByCropping(HDC hdc, int x, int y, int width, int height, int sx, int sy, int sw, int sh) const
{
	HDC hMemDC;

	hMemDC = CreateCompatibleDC(hdc);

	SelectObject(hMemDC, m_BitmapHandle);

	TransparentBlt(
		hdc, x, y,
		width == 0 ? m_BitmapInfo.bmWidth : width,
		height == 0 ? m_BitmapInfo.bmHeight : height,
		hMemDC, sx, sy, sw, sh, getTransparentColor());

	DeleteDC(hMemDC);
}

void CBitmap::setTransparentColor(DWORD color)
{
	m_TransparentColor = color;
}

DWORD CBitmap::getTransparentColor()
{
	return m_TransparentColor;
}