#pragma once

#include <Windows.h>
#include <string>
#include <map>

class CScene;

class CSceneManager
{
public:

	CSceneManager( );
	~CSceneManager( );

public:

	void					registerScene( const std::string& sceneName, CScene* scene );
	void					reserveChangeScene( const std::string& sceneName );

public:

	void					update( float dt );
	void					render( HDC hdc, float dt );

	// 마우스 컨드롤
	void					OnMouseUp(WPARAM btnState, int x, int y);
	void					OnMouseDown(WPARAM btnState, int x, int y);
	void					OnMouseWheel(WPARAM btnState, int x, int y);
	void					OnMouseMove(WPARAM btnState, int x, int y);

private:

	std::map<std::string,CScene*>	m_SceneContainer;
	
	CScene*					m_ReservedScene;
	CScene*					m_CurrentScene;

};
