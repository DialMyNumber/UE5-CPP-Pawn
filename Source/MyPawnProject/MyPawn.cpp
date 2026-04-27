#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPawnController.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 2000.f; // SpringArm 기본 거리
	SpringArmComp->SocketOffset = FVector(0.f, 0.f, 500.f);	// SocketOffset 기본 거리
	SpringArmComp->bUsePawnControlRotation = true; // Pawn을 컨트롤 할 때 SpringArm도 같이 이동
	SpringArmComp->bInheritPitch = false;	// SpringArm에 Pawn의 Pitch 적용 x
	SpringArmComp->bInheritRoll = false;	// SpringArm에 Pawn의 Roll 적용 x

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// USpringArmComponent::SocketName 이라고 정의되어 있는 끝부분에 부착
	CameraComp->bUsePawnControlRotation = false;
	// 카메라 자체는 움직이지 않고 SpringArm에 고정되어 있도록 비활성화

	MoveSpeed = 1000.f;
	VerticalSpeed = 500.f;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPawnController* PlayerController = Cast<AMyPawnController>(GetController()))
		{
			if (PlayerController->MoveAction)	// nullptr 인지 확인 후
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,	// 만들었던 IA MoveAction 에
					ETriggerEvent::Triggered,	// Key가 눌려서 Event가 발생했을때
					this,	// 현재 객체의
					&AMyPawn::PMove	// PMove 함수를 IA MoveAction 에 바인딩
				);
			}


			if (PlayerController->AltitudeAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AltitudeAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::PUpDown
				);
			}

			if (PlayerController->YawAction)
			{
				EnhancedInput->BindAction(
					PlayerController->YawAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::PYaw
				);
			}

			UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetNameSafe(GetController()));

		}
	}
}

void AMyPawn::PMove(const FInputActionValue& value)
{
	FVector2D Input = value.Get<FVector2D>();

	if (!Input.IsNearlyZero())
	{
		FVector MoveDir = FVector(Input.X, Input.Y, 0.f);
		AddActorLocalOffset(MoveDir * MoveSpeed * GetWorld()->DeltaTimeSeconds, true);
	}
}

void AMyPawn::PUpDown(const FInputActionValue& value)
{
	const float Input = value.Get<float>();

	if (!FMath::IsNearlyZero(Input))
	{
		FVector Move = GetActorUpVector() * Input;

		AddActorWorldOffset(Move * VerticalSpeed * GetWorld()->GetDeltaSeconds(), true);
	}
}

void AMyPawn::PYaw(const FInputActionValue& value)
{
	const float Input = value.Get<float>();

	if (!FMath::IsNearlyZero(Input))
	{
		AddActorLocalRotation(FRotator(0.f, Input * 0.2f, 0.f));
	}
}