//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple first person style camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be constructed.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
//***************************************************************************************

#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

class Camera
{
public:
	Camera();
	~Camera();

	// ���� ���� ī�޶� ��ġ�� ��ȸ �Ǵ� ����
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	
	// ī�޶� �������͵��� �����ش�.
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;

	// ����ü �Ӽ����� �����ش�.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// �þ� ���� ������ �����/�� ��� ũ�⸦ �����ش�.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;
	
	// ����ü�� �����Ѵ�.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// ī�޶� ��ġ�� �ü� �� ���� �������� ī�޶� ������ �����Ѵ�.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// �þ� ��İ� ��������� �����ش�.
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	// ī�޶� �Ÿ� d��ŭ Ⱦ�̵�, ���̵��Ѵ�.
	void Strafe(float d);
	void Walk(float d);

	// ī�޶� ȸ���Ѵ�.
	void Pitch(float angle);
	void RotateY(float angle);
	void Roll(float angle);

	// �� �����Ӹ���, ī�޶� ��ġ�� ������ ������ ��
	// �� �޼��带 ȣ���ؼ� �þ� ����� �籸���Ѵ�.
	void UpdateViewMatrix();

private:

	// ���� ���� ������ ī�޶� ��ǥ��
	XMFLOAT3 mPosition; // �þ� ���� ����
	XMFLOAT3 mRight; // �þ� ���� x ��
	XMFLOAT3 mUp; // �þ� ���� y ��
	XMFLOAT3 mLook; // �þ� ���� z ��

	// ����ü �Ӽ���
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// �þ� ��İ� ���� ���
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};

#endif // CAMERA_H