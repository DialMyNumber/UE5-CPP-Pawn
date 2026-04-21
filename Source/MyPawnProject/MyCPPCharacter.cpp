#include "MyCPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyCPPCharacter::AMyCPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.f; // SpringArm 기본 거리 
	SpringArmComp->bUsePawnControlRotation = true;
	// Pawn을 컨트롤 할 때 SpringArm도 같이 이동

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// USpringArmComponent::SocketName 이라고 정의되어 있는 끝부분에 부착
	CameraComp->bUsePawnControlRotation = false;	
	// 카메라 자체는 움직이지 않고 SpringArm에 고정되어 있도록 비활성화
}

void AMyCPPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}