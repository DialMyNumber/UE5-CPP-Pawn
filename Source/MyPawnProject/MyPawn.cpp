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
	SpringArmComp->TargetArmLength = 3500.f; // SpringArm 기본 거리
	SpringArmComp->SocketOffset = FVector(0.f, 0.f, 600.f);	// SocketOffset 기본 거리
	SpringArmComp->bUsePawnControlRotation = true; // Pawn을 컨트롤 할 때 SpringArm도 같이 이동
	SpringArmComp->bInheritPitch = false;	// SpringArm에 Pawn의 Pitch 적용 x
	SpringArmComp->bInheritRoll = false;	// SpringArm에 Pawn의 Roll 적용 x

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// USpringArmComponent::SocketName 이라고 정의되어 있는 끝부분에 부착
	CameraComp->bUsePawnControlRotation = false;
	// 카메라 자체는 움직이지 않고 SpringArm에 고정되어 있도록 비활성화


	Velocity = FVector::ZeroVector;

	MoveAccel = 500.f;
	VerticalAccel = 400.f;
	Drag = 0.55f;
	MaxSpeed = 2000.f;

	YawSpeed = 30.f;
	CurrentYawInput = 0.f;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 속도 제한
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

	// 이동
	AddActorWorldOffset(Velocity * DeltaTime, true);

	// 감속 (공기 저항 느낌)
	Velocity *= Drag;
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

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::PLook
				);
			}
		}
	}
}

void AMyPawn::PMove(const FInputActionValue& value)
{
	FVector2D Input = value.Get<FVector2D>();

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	FVector Direction = (Forward * Input.Y) + (Right * Input.X);

	Velocity += Direction * MoveAccel;
}

void AMyPawn::PUpDown(const FInputActionValue& value)
{
	const float Input = value.Get<float>();

	FVector Up = GetActorUpVector();

	Velocity += Up * Input * VerticalAccel;
}

void AMyPawn::PYaw(const FInputActionValue& value)
{
	const float Input = value.Get<float>();

	if (!FMath::IsNearlyZero(Input))
	{
		// 회전은 입력값만큼 즉시 반영
		AddActorLocalRotation(FRotator(0.f, Input * YawSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
	}
}

void AMyPawn::PLook(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	// 스프링암의 현재 회전 값을 가져오기
	FRotator CurrentRotation = SpringArmComp->GetComponentRotation();

	// Yaw (좌우 회전) 적용
	float DeltaYaw = LookInput.X * 0.5f;
	CurrentRotation.Yaw += DeltaYaw;

	// Pitch (상하 회전) 적용
	float DeltaPitch = LookInput.Y * 0.2f;
	CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + DeltaPitch, -80.f, 80.f); // Pitch 제한

	// 수정된 회전 값을 스프링암에 적용
	SpringArmComp->SetWorldRotation(CurrentRotation);
}