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
	// 뼈대 그리는 레스터화기 상태
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// 통상적인 레스터화기 상태, 후면 제거는 안했다.
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// 거울 반대변에 그려질 물건에 대한 레스터화기 상태
	//

	// 주의: 반시계 방향을 전면 삼각형으로 간주해서 후면 삼각형들을 선별한다.
	// 후면 선별을 비활성화하는 것이 아님을 주의할 것.
	// 후면 선별을 하지 않는다면 D3D11_DEPTH_STENCIL_DESC의 BackFace 속성에 신경을 써야 한다.
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
	// 투명 혼합 상태
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
	// 거울의 깊이, 스텐실 버퍼 상태 설정
	// 거울은 일단 깊이 버퍼 쓰기, 후면 버퍼 쓰기를 비활성화하고 오직 스텐실에만 기록되게 해야한다.
	// 스텐실 이외의 쓰기 기능을 끄는 이유는 반대편에서 비출 해골을 거울에 비친 것마냥 보여야 하기 때문이다.
	// 거울이 스텐실 버퍼에 렌더링 될 때는 스텐실 버퍼 판정이 항상 성공하도록 
	// D3D11_COMPARISON_ALWAYS 이 설정을 해야한다.
	// 즉, 깊이 버퍼 판정에 따라서 스텐실 버퍼 값이 0, 1 둘 중에 하나가 된다.
	//

	D3D11_DEPTH_STENCIL_DESC mirrorDesc; // 깊이, 스텐실 상태 집합
	mirrorDesc.DepthEnable = true; // 깊이 버퍼 활성화 유무, false 라면 그리는 순서 중요
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 버퍼 쓰기 활성화 유무, 깊이 판정은 유지
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS; // 깊이 판정 연산자 종류 설정
	mirrorDesc.StencilEnable = true; // 스텐실 판정 활성화 유무
	mirrorDesc.StencilReadMask = 0xff; // 특정 스텐실 비트를 막지 않고 읽거나 막고 읽거나 설정
	mirrorDesc.StencilWriteMask = 0xff; // 스텐실 버퍼 갱신할 때 특정 비트 값이 갱신되지 않게 할 수 있음

	// 전면 삼각형에 대한 스텐실 버퍼 적용 방식 설정
	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // 픽셀 단면이 스텐실 판정에 실패했을 때의 스텐실 버퍼의 갱신 방식 설정
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // 스텐실 판정 통과, 깊이 판정 실패의 경우 스텐실 버퍼의 갱신 방법 설정
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE; // 스텐실, 깊이 판정 모두 통과 시에 스텐실 버퍼의 갱신 방법을 설정
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // 스텐실 판정 비교 함수를 결정하는 멤버

	// 후면 삼각형에 대한 스텐실 버퍼 적용 방식 설정, 보통 후면 제거를 하기에 안쓰인다.
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS)); // 위와 같이 설정된 스텐실깊이 상태를 MarkMirrorDSS에 담아둔다.

	//
	// 거울에 반사된 물체에 대한 깊이, 스텐실 상태 설정
	// 이 상태를 쓰는 시점에서 스텐실 버퍼는 거울 영역에 1로 표시되어 있다.
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
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; // 스텐실이 1이여야 정상적으로 그려진다.

	// We are not rendering backfacing polygons, so these settings do not matter.
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS));

	//
	// 이중 혼합을 방지하는 깊이 스텐실 상태.
	// 그림자를 그리는 경우 사용하게 된다.
	// 왜 이중 혼합을 방지하냐면 한 물체에 대한 그림자를 나타내는데
	// 그림자를 그리는 대상 물체의 삼각형 영역들이 겹쳐져서 그림자에 얼룩이 지는 경우가 있다.
	// 투영시에 중복되는 정점이 있다는 말이다.
	// 따라서 이를 방지하기 위해 필요하다.
	// 어떻게 이중 혼합을 방지하냐면 스텐실 버퍼를 처음에 0으로 했다가 한번 그림자 출력을 하면서
	// 해당 픽셀을 거쳤다면 스텐실 버퍼 값을 증가시키면서 다음에 다시 그 픽셀을 건드리려하는 것을
	// 방지한다. 즉 스텐실 버퍼가 0인 경우만 그림자를 생성하는 것이다.
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
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; // 스텐실과 깊이 버퍼 판정이 모두 성공했으면 스텐실 버퍼 증가시킴
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL; // 0으로 같으면 픽셀을 그림자 색으로 혼합

	// 후면 삼각형에 대한 스텐실 버퍼 적용 방식 설정, 보통 후면 제거를 하기에 안쓰인다.
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