#include "MyCPPGameMode.h"
#include "MyCPPCharacter.h"	// 기본 캐릭터로 지정할 헤더파일 

AMyCPPGameMode::AMyCPPGameMode()
{
	DefaultPawnClass = AMyCPPCharacter::StaticClass();
	// StaticClass : 객체 생성 없이 클래스를 반환
}