#include "SceneManager.h"
#include "Scene.h"

using namespace std;

CSceneManager::CSceneManager( ) : m_CurrentScene( nullptr ), m_ReservedScene( nullptr )
{
}

CSceneManager::~CSceneManager( )
{
	for( auto it = m_SceneContainer.begin( ); it != m_SceneContainer.end( ); ++it )
	{
		it->second->destroy( );
		delete it->second;
	}
}

void CSceneManager::registerScene( const std::string& sceneName, CScene* scene )
{
	if( scene == nullptr || sceneName.compare( "" ) == 0 )
		return;

	scene->create( );

	m_SceneContainer.insert( m_SceneContainer.end( ), pair<string, CScene*>( sceneName, scene ) );
}

void CSceneManager::reserveChangeScene( const std::string& sceneName )
{
	auto it = m_SceneContainer.find( sceneName );

	if( it != m_SceneContainer.end( ) )
	{
		m_ReservedScene = it->second;
	}
	else
		m_ReservedScene = nullptr;
}

void CSceneManager::update( float dt )
{
	if( m_ReservedScene != nullptr )
	{
		if( m_CurrentScene != nullptr )
			m_CurrentScene->clear( );

		m_ReservedScene->initialize( );
		m_CurrentScene = m_ReservedScene;

		m_ReservedScene = nullptr;
	}

	if( m_CurrentScene != nullptr )
		m_CurrentScene->update( dt );
}

void CSceneManager::render( HDC hdc, float dt )
{
	if( m_CurrentScene != nullptr )
		m_CurrentScene->render( hdc, dt );
}

void CSceneManager::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (m_CurrentScene != nullptr)
		m_CurrentScene->OnMouseUp(btnState, x, y);
}

void CSceneManager::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (m_CurrentScene != nullptr)
		m_CurrentScene->OnMouseDown(btnState, x, y);
}