#pragma once
#include "CD3DApp.h"
#include "CAxis.h"
#include "CCube.h"
#include "CGround.h"

// 어떤 도형이나 물건을 출력할 때에는 실제 필요한 값들을 묶어서 표현해야 코드가 깔끔해진다.
struct SPHERE_PROPERTY // 출력을 위한 속성 묶음
{
	D3DXVECTOR3 vTans; // 중심점과 같은 역할
	float fScaling; // 스케일(크기)
	float fRadius; // 반지름
};

class CGameEdu01 : public CD3DApp
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj; // 투영 변환을 위한 행렬
	D3DXVECTOR3 m_Eye, m_At, m_Up; // 뷰 변환을 위한 위치 (카메라 위치, 보는 시점)

	// 폰트
	LPD3DXFONT m_pFont;

	SPHERE_PROPERTY m_Sphere[2]; // 충돌 체크를 위한 구 2개
	BOOL m_bIsCollision; // 충돌인지 아닌지 BOOL형
	LPD3DXMESH m_pMesh; // 도형 출력 매쉬

	DWORD m_dwElapsedTime; // 지나간 시간
	unsigned int m_nFPS, m_nFPSCount; // 1초가 되었을 경우 출력되는 FPS, 1초 되기 전까지 카운트되는 FPS 값

private:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGameEdu01(void);
	~CGameEdu01(void);
};

