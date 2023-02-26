//=============================================================================
// Blur.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Performs a separable blur with a blur radius of 5.  
//=============================================================================

cbuffer cbSettings
{
	float gWeights[11] = // �帮�� ����ġ��
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
	};
};

cbuffer cbFixed
{
	static const int gBlurRadius = 5; // �帮�� �ȼ� ������ 5�� �ؼ� + ���� �ؼ� + 5�� �ؼ�
};

Texture2D gInput; // �б� ���� �ڿ�
RWTexture2D<float4> gOutput; // �б� ���� ��� ������ ��� �ڿ�, UAV ����

#define N 256
#define CacheSize (N + 2*gBlurRadius)
// ���� �׷� ������ �����ϴ� ���� �޸� ����, ���� 4���� float4
// gInput���� �׶� �׶� �ؼ� ���� ������ ������� �ʰ� ���⿡ ������ ����ϴ� ������
// ���� �޸� ������ �ξ� �����⿡ �� ȿ�����̶� �׷���.
// ũ��� �帮�� ó���� �˸��� R + N + R ���ʿ� ������ ��ŭ �߰�
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)] // �׷� �ϳ��� ������ 256��
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    // �⺻ �������� gInput.Length.x �̷� ������ ����ߴµ�
	// �̷� ����� ���� ��������.
	// �ؽ��� ���̸� ȹ���ϴ� �Լ��� ã�ƺôµ� GetDimensions
	// �Լ��� �����Ͽ� �̸� ����ؼ� ���ƴ�.
	int W, H; // ����, ���� ����
    gInput.GetDimensions(W, H);
	
	//
	// �뿪���� ���̱� ���� ���� ������ ����Ҹ� ä���.
	// �帮�� ������ ������, �ȼ� N���� �帮���� N + 2 * �帮�� ������
	// ��ŭ�� �ȼ��� �о�� �Ѵ�.
	//

	// �� ������ �׷��� N���� �����带 �����Ѵ�.
	// ������ �ȼ� 2 * �帮�� ������ ���� ����, 
	// 2 * �帮�� ������ ��ŭ�� �����尡 �ȼ��� �ϳ� �� �����ϰ� �Ѵ�.
	
	// ���� ������ ID�� ���������� ������ �帲ó�� �ÿ� 256 ������ �����
	// �ؼ��� �ʿ��ϴ�. ��Ȯ�� ���ϸ� �� ���� �κ��� �ʿ��ؼ�
	// �ذ� ���� �۾��� ����ȴ�.
	if(groupThreadID.x < gBlurRadius)
	{
		// �̹��� ���� ��� �ٱ��� ǥ���� ���� ���� ����
		// �� ��ġ���� �������� �� ��ġ�� �ؼ� ���� ���� dispatchThreadID.x - gBlurRadius �κ� �ƴϸ� ��� �κ�
        int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
	}
	
	// Ŭ ���� ���������� �� ������ �κ��� �ʿ������⿡
	// ���� �۾� ����
	if(groupThreadID.x >= N-gBlurRadius)
    {
		// �̹��� ������ ��� �ٱ��� ǥ���� ������ ���� ����
        int x = min(dispatchThreadID.x + gBlurRadius, W - 1);
		gCache[groupThreadID.x+2*gBlurRadius] = gInput[int2(x, dispatchThreadID.y)];
	}

	// �̹��� ���� ���� ǥ���� �̹��� ���� �����Ѵ�.
	// �̹��� ũ�⸦ ��������� �����尡 �����ϴ� ��찡 ����.
	// �� ������ ���� ũ�⸦ 256���� �������⿡ ���� ũ�Ⱑ 256�� ����� �ƴ϶��
	// �������� ���� �װ� �� ������ ��ŭ�� �����忡 �̹��� ���� ������ ��ϵǸ� �ȵǱ⿡
	// min(dispatchThreadID.xy, gInput.Length.xy-1) �� ������ �ʿ��ϴ�.
    gCache[groupThreadID.x + gBlurRadius] = gInput[min(dispatchThreadID.xy, (W - 1) * (H - 1))];

	// �ٸ� �׷� ���� ������鵵 ������� ������ ������ ���� �ܰ�� �Ѿ��
	// ����ȭ ��Ų��.
	GroupMemoryBarrierWithGroupSync();
	
	//
	// �帲 ó�� ����
	//

	float4 blurColor = float4(0, 0, 0, 0);
	
	[unroll]
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
        // gCache[]�� groupThreadID.x ���� ������ �����̰�
		// groupThreadID.x + 2*R ���� ������ �������̶� �ε����� �ٽ� �����.
		int k = groupThreadID.x + gBlurRadius + i; // �ε��� �缳��
		
		blurColor += gWeights[i+gBlurRadius]*gCache[k];
	}
	
	gOutput[dispatchThreadID.xy] = blurColor;
}

// ���� ���� �帮�⵵ ���⸸ �ٸ� �� �׳� �Ȱ���.
[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
    int W, H; // ����, ���� ����
    gInput.GetDimensions(W, H);
	
	//
	// �뿪���� ���̱� ���� ���� ������ ����Ҹ� ä���.
	// �帮�� ������ ������, �ȼ� N���� �帮���� N + 2 * �帮�� ������
	// ��ŭ�� �ȼ��� �о�� �Ѵ�.
	//
	
	// �� ������ �׷��� N���� �����带 �����Ѵ�.
	// ������ �ȼ� 2 * �帮�� ������ ���� ����, 
	// 2 * �帮�� ������ ��ŭ�� �����尡 �ȼ��� �ϳ� �� �����ϰ� �Ѵ�.
	
	// ���� ������ ID�� ���������� ������ �帲ó�� �ÿ� 256 ������ �����
	// �ؼ��� �ʿ��ϴ�. ��Ȯ�� ���ϸ� �� ���� �κ��� �ʿ��ؼ�
	// �ذ� ���� �۾��� ����ȴ�.
	if(groupThreadID.y < gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int y = max(dispatchThreadID.y - gBlurRadius, 0);
		gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
	}
	if(groupThreadID.y >= N-gBlurRadius)
	{	
		// Clamp out of bound samples that occur at image borders.
        int y = min(dispatchThreadID.y + gBlurRadius, H - 1);
		gCache[groupThreadID.y+2*gBlurRadius] = gInput[int2(dispatchThreadID.x, y)];
	}
	
	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.y + gBlurRadius] = gInput[min(dispatchThreadID.xy, (W - 1) * (H - 1))];


	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();
	
	//
	// Now blur each pixel.
	//

	float4 blurColor = float4(0, 0, 0, 0);
	
	[unroll]
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.y + gBlurRadius + i;
		
		blurColor += gWeights[i+gBlurRadius]*gCache[k];
	}
	
	gOutput[dispatchThreadID.xy] = blurColor;
}

technique11 HorzBlur
{
    pass P0
    {
		SetVertexShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, HorzBlurCS() ) );
    }
}

technique11 VertBlur
{
    pass P0
    {
		SetVertexShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, VertBlurCS() ) );
    }
}
