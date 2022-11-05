#ifndef SKINNEDDATA_H
#define SKINNEDDATA_H

#include "d3dUtil.h"
#include <map>

///<summary>
/// A Keyframe defines the bone transformation at an instant in time.
///</summary>
struct Keyframe
{
	Keyframe();
	~Keyframe();

    float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

///<summary>
/// A BoneAnimation is defined by a list of keyframes.  For time
/// values inbetween two keyframes, we interpolate between the
/// two nearest keyframes that bound the time.  
///
/// We assume an animation always has two keyframes.
/// ���� �� ���� ���� �ִϸ��̼�
///</summary>
struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

    void Interpolate(float t, XMFLOAT4X4& M)const;

	std::vector<Keyframe> Keyframes; 	

};

///<summary>
/// �ȱ�, �ٱ�, ����, ��� ���� ���� �ִϸ��̼� Ŭ���� ��ǥ�Ѵ�.
/// �ϳ��� AnimationClip�� �ִϸ��̼� Ŭ���� �����ϴ�
/// ����� �ϳ����� BoneAnimation �ν��Ͻ��� ��´�. 
///</summary>
struct AnimationClip
{
	// Ŭ���� ��� ���� �� ���� �̸� ���� �ð��� �����ش�.
	float GetClipStartTime()const;
	// Ŭ���� ��� ���� �� ���� ���� ���� �ð��� �����ش�.
	float GetClipEndTime()const;
	// �� Ŭ�� �ð� ���̸� �������� ȹ���ϱ� ���� �Լ�
    void Interpolate(float t, std::vector<XMFLOAT4X4>& boneTransforms)const;

	// ���뺰 �ִϸ��̼ǵ�.
    std::vector<BoneAnimation> BoneAnimations; 	
};

class SkinnedData
{
public:

	UINT BoneCount()const;

	float GetClipStartTime(const std::string& clipName)const;
	float GetClipEndTime(const std::string& clipName)const;

	void Set(
		std::vector<int>& boneHierarchy, 
		std::vector<XMFLOAT4X4>& boneOffsets,
		std::map<std::string, AnimationClip>& animations);

	// ���� ������Ʈ�� ���, ���� ���� clipName�� timePos��
	// �� �޼��带 ���� �� ȣ���� ���ɼ��� ũ�ٸ� ������ ĳ��
	// �ý����� �����ϴ� ���� �ٶ��� �� ���̴�.
    void GetFinalTransforms(const std::string& clipName, float timePos, 
		 std::vector<XMFLOAT4X4>& finalTransforms)const;

private:
	// ������ �迭�� Ʈ�� ���� ����
    // i�� ������ �θ��� ������ ��´�. i�� �����
	// �ִϸ��̼� Ŭ���� i��° BoneAnimation �ν��Ͻ��� �����ȴ�.
	std::vector<int> mBoneHierarchy;
	// i��° ���� ������ ��ȯ
	std::vector<XMFLOAT4X4> mBoneOffsets;
   
	std::map<std::string, AnimationClip> mAnimations;
};
 
#endif // SKINNEDDATA_H