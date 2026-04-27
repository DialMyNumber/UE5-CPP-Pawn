#include "MyDroneGameMode.h"
#include "MyPawn.h"	// 기본 캐릭터로 지정할 헤더파일 
#include "MyPawnController.h" // 기본 플레이어 컨트롤러로 지정할 헤더파일

AMyDroneGameMode::AMyDroneGameMode()
{
	// StaticClass : 객체 생성 없이 클래스를 반환
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPawnController::StaticClass();
}