#include "GameDirector.h"
#include "SceneManager.h"
#include "HelloWord.h"
#include "PrepareWnd.h"

#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

void settingWnd(CPrepareWnd& value)
{
	value.lpWindowName = "tongstar";
}

// registerScene���� ��... ����ȭ��, ���� �÷��̽� ȭ��, ���丮 �ڸ� ȭ��, �ε� ȭ�� ����
// �̸� �����س��� getSceneManager�� ���� ���� �����Ѵ�.
void settingGame()
{
	CGameDirector* director = CGameDirector::getGameDirector();

	director->getSceneManager()->registerScene("helloworld", new HelloWord);
	director->getSceneManager()->reserveChangeScene("helloworld");
}