//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::WireframeRS = 0;
ID3D11RasterizerState* RenderStates::NoCullRS = 0;
ID3D11RasterizerState* RenderStates::CullClockwiseRS = 0;

ID3D11BlendState* RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState* RenderStates::TransparentBS = 0;
ID3D11BlendState* RenderStates::NoRenderTargetWritesBS = 0;

ID3D11DepthStencilState* RenderStates::MarkMirrorDSS = 0;
ID3D11DepthStencilState* RenderStates::DrawReflectionDSS = 0;
ID3D11DepthStencilState* RenderStates::NoDoubleBlendDSS = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// ���� �׸��� ������ȭ�� ����
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// ������� ������ȭ�� ����, �ĸ� ���Ŵ� ���ߴ�.
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// �ſ� �ݴ뺯�� �׷��� ���ǿ� ���� ������ȭ�� ����
	//

	// ����: �ݽð� ������ ���� �ﰢ������ �����ؼ� �ĸ� �ﰢ������ �����Ѵ�.
	// �ĸ� ������ ��Ȱ��ȭ�ϴ� ���� �ƴ��� ������ ��.
	// �ĸ� ������ ���� �ʴ´ٸ� D3D11_DEPTH_STENCIL_DESC�� BackFace �Ӽ��� �Ű��� ��� �Ѵ�.
	D3D11_RASTERIZER_DESC cullClockwiseDesc;
	ZeroMemory(&cullClockwiseDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullClockwiseDesc.FillMode = D3D11_FILL_SOLID;
	cullClockwiseDesc.CullMode = D3D11_CULL_BACK;
	cullClockwiseDesc.FrontCounterClockwise = true;
	cullClockwiseDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&cullClockwiseDesc, &CullClockwiseRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// ���� ȥ�� ����
	//

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	//
	// NoRenderTargetWritesBS
	//

	D3D11_BLEND_DESC noRenderTargetWritesDesc = { 0 };
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;

	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	HR(device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS));

	//
	// �ſ��� ����, ���ٽ� ���� ���� ����
	// �ſ��� �ϴ� ���� ���� ����, �ĸ� ���� ���⸦ ��Ȱ��ȭ�ϰ� ���� ���ٽǿ��� ��ϵǰ� �ؾ��Ѵ�.
	// ���ٽ� �̿��� ���� ����� ���� ������ �ݴ����� ���� �ذ��� �ſ￡ ��ģ �͸��� ������ �ϱ� �����̴�.
	// �ſ��� ���ٽ� ���ۿ� ������ �� ���� ���ٽ� ���� ������ �׻� �����ϵ��� 
	// D3D11_COMPARISON_ALWAYS �� ������ �ؾ��Ѵ�.
	// ��, ���� ���� ������ ���� ���ٽ� ���� ���� 0, 1 �� �߿� �ϳ��� �ȴ�.
	//

	D3D11_DEPTH_STENCIL_DESC mirrorDesc; // ����, ���ٽ� ���� ����
	mirrorDesc.DepthEnable = true; // ���� ���� Ȱ��ȭ ����, false ��� �׸��� ���� �߿�
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ���� ���� Ȱ��ȭ ����, ���� ������ ����
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS; // ���� ���� ������ ���� ����
	mirrorDesc.StencilEnable = true; // ���ٽ� ���� Ȱ��ȭ ����
	mirrorDesc.StencilReadMask = 0xff; // Ư�� ���ٽ� ��Ʈ�� ���� �ʰ� �аų� ���� �аų� ����
	mirrorDesc.StencilWriteMask = 0xff; // ���ٽ� ���� ������ �� Ư�� ��Ʈ ���� ���ŵ��� �ʰ� �� �� ����

	// ���� �ﰢ���� ���� ���ٽ� ���� ���� ��� ����
	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // �ȼ� �ܸ��� ���ٽ� ������ �������� ���� ���ٽ� ������ ���� ��� ����
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // ���ٽ� ���� ���, ���� ���� ������ ��� ���ٽ� ������ ���� ��� ����
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // ���ٽ�, ���� ���� ��� ��� �ÿ� ���ٽ� ������ ���� ����� ����
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // ���ٽ� ���� �� �Լ��� �����ϴ� ���

	// �ĸ� �ﰢ���� ���� ���ٽ� ���� ���� ��� ����, ���� �ĸ� ���Ÿ� �ϱ⿡ �Ⱦ��δ�.
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS)); // ���� ���� ������ ���ٽǱ��� ���¸� MarkMirrorDSS�� ��Ƶд�.

	//
	// �ſ￡ �ݻ�� ��ü�� ���� ����, ���ٽ� ���� ����
	// �� ���¸� ���� �������� ���ٽ� ���۴� �ſ� ������ 1�� ǥ�õǾ� �ִ�.
	//

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	drawReflectionDesc.DepthEnable = true;
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; // ���ٽ��� 1�̿��� ���������� �׷�����.

	// We are not rendering backfacing polygons, so these settings do not matter.
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS));

	//
	// ���� ȥ���� �����ϴ� ���� ���ٽ� ����.
	// �׸��ڸ� �׸��� ��� ����ϰ� �ȴ�.
	// �� ���� ȥ���� �����ϳĸ� �� ��ü�� ���� �׸��ڸ� ��Ÿ���µ�
	// �׸��ڸ� �׸��� ��� ��ü�� �ﰢ�� �������� �������� �׸��ڿ� ����� ���� ��찡 �ִ�.
	// �����ÿ� �ߺ��Ǵ� ������ �ִٴ� ���̴�.
	// ���� �̸� �����ϱ� ���� �ʿ��ϴ�.
	// ��� ���� ȥ���� �����ϳĸ� ���ٽ� ���۸� ó���� 0���� �ߴٰ� �ѹ� �׸��� ����� �ϸ鼭
	// �ش� �ȼ��� ���ƴٸ� ���ٽ� ���� ���� ������Ű�鼭 ������ �ٽ� �� �ȼ��� �ǵ帮���ϴ� ����
	// �����Ѵ�. �� ���ٽ� ���۰� 0�� ��츸 �׸��ڸ� �����ϴ� ���̴�.
	//

	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable = true;
	noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	noDoubleBlendDesc.StencilEnable = true;
	noDoubleBlendDesc.StencilReadMask = 0xff;
	noDoubleBlendDesc.StencilWriteMask = 0xff;

	noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; // ���ٽǰ� ���� ���� ������ ��� ���������� ���ٽ� ���� ������Ŵ
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; // 0���� ������ �ȼ��� �׸��� ������ ȥ��

	// �ĸ� �ﰢ���� ���� ���ٽ� ���� ���� ��� ����, ���� �ĸ� ���Ÿ� �ϱ⿡ �Ⱦ��δ�.
	noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(CullClockwiseRS);

	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(NoRenderTargetWritesBS);

	ReleaseCOM(MarkMirrorDSS);
	ReleaseCOM(DrawReflectionDSS);
	ReleaseCOM(NoDoubleBlendDSS);
}