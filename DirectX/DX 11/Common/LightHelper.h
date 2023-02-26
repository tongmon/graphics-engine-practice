//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include <xnamath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DirectionalLight // ������(�¾籤)
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient; // �ֺ���
	XMFLOAT4 Diffuse; // �� ����
	XMFLOAT4 Specular; // �ݿ���(��ü�� ���� ���ϰ� ���߸� ����� �� �� �κ�)
	XMFLOAT3 Direction; // ����
	float Pad; // �ʿ��ϴٸ� ������ �迭�� ������ �� �ְ� �������� float �ϳ��� �ڸ��� ������.
};

struct PointLight // ����(�鿭����)
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	// (Position, Range) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����.
	XMFLOAT3 Position; // �� ��
	float Range; // �� ����

	// (A0, A1, A2, Pad) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����.
	// ���� ���Ⱑ �Ÿ��� ���� �����ϴµ� �� �Ŀ��� 3���� ����� �ǹ�
	// ������ ���� ���⸦ A0 + A1 * �Ÿ� + A2 * �Ÿ� * �Ÿ��� �� ���� ������ ������
	XMFLOAT3 Att;
	float Pad; // �ʿ��ϴٸ� ������ �迭�� ������ �� �ְ� �������� float �ϳ��� �ڸ��� ������.
};

struct SpotLight // ����Ʈ����Ʈ(������, �������� ���� ����)
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	// (Position, Range) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����.
	// ������ ����
	XMFLOAT3 Position;
	float Range;

	// (Direction, Spot) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����.
	// �������� ���� �߰�
	XMFLOAT3 Direction;
	float Spot; // �������� �� ������ ���� ����ε� �̸� ����

	// (Att, Pad) ���·� �ϳ��� 4���� ���Ϳ� ä�� ����.
	XMFLOAT3 Att;
	float Pad; // �ʿ��ϴٸ� ������ �迭�� ������ �� �ְ� �������� float �ϳ��� �ڸ��� ������.
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
};

#endif // LIGHTHELPER_H