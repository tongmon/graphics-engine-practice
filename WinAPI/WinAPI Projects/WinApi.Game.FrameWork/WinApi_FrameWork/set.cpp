#include "GameDirector.h"
#include "SceneManager.h"
#include "HelloWord.h"
#include "PrepareWnd.h"

#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

void settingWnd(CPrepareWnd& value)
{
	value.lpWindowName = "tongstar";
}

// registerScene으로 뭐... 메인화면, 게임 플레이시 화면, 스토리 텔링 화면, 로딩 화면 등을
// 미리 세팅해놓고 getSceneManager로 현재 씬을 설정한다.
void settingGame()
{
	CGameDirector* director = CGameDirector::getGameDirector();

	director->getSceneManager()->registerScene("helloworld", new HelloWord);
	director->getSceneManager()->reserveChangeScene("helloworld");
}