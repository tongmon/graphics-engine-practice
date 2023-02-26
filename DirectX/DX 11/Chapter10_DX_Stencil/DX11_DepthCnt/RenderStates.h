//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include "d3dUtil.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// ������ȭ�� �ܰ� ���� 2��
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;

	// ȥ�� ���� ���� 2��
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;

	// Depth/stencil ����
	static ID3D11DepthStencilState* NormalDSS;
	static ID3D11DepthStencilState* IncStencilDSS;
	static ID3D11DepthStencilState* DistStencilDSS;
};

#endif // RENDERSTATES_H