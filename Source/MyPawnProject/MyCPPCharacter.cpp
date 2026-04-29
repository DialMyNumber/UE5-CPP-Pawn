#include "MyCPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCPPCharacter::AMyCPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

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

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	// 현재 Character의 MaxWalkSpeed 를 NormalSpeed로 설정
}

void AMyCPPCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerInventory.AddItem("Wood Sword");
	PlayerInventory.AddItem("Iron Sword");
	PlayerInventory.AddItem("Healing Potion");
	PlayerInventory.AddItem("Iron Helmet");
	PlayerInventory.AddItem("Healing Potion");
	PlayerInventory.AddItem("Long Bow");

	UE_LOG(LogTemp, Log, TEXT("%s's Inventory Array List"), *GetName());
	PlayerInventory.ShowInventoryArr();

	UE_LOG(LogTemp, Log, TEXT("%s's Inventory Map List"), *GetName());
	PlayerInventory.ShowInventoryMap();

	PlayerTitle.AddTitle(TEXT("10살에 곰을 잡은"));
	PlayerTitle.AddTitle(TEXT("여신을 구출한"));

	UE_LOG(LogTemp, Log, TEXT("%s's Title List"), *GetName());
	PlayerTitle.PrintAllTitles();
	
	if(PlayerTitle.isAchieved(TEXT("10살에 곰을 잡은"))) {
		UE_LOG(LogTemp, Log, TEXT("Wood Sword 장착 가능"));
	}

	if (PlayerTitle.isAchieved(TEXT("여신을 구출한"))) {
		UE_LOG(LogTemp, Log, TEXT("Iron Sword 장착 가능"));
	}

	if (PlayerTitle.isAchieved(TEXT("솜씨좋은"))) {
		UE_LOG(LogTemp, Log, TEXT("Long Bow 장착 가능"));
	}
}

void AMyCPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)	// nullptr 인지 확인 후
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,	// 만들었던 IA MoveAction 에
					ETriggerEvent::Triggered,	// Key가 눌려서 Event가 발생했을때
					this,	// 현재 Character 객체의
					&AMyCPPCharacter::Move	// Move 함수를 IA MoveAction 에 바인딩
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCPPCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCPPCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCPPCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCPPCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCPPCharacter::StopSprint
				);
			}
		}
	}
}

void AMyCPPCharacter::Move(const FInputActionValue& value)	// Axis2D = 2D Vector
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	FRotator ControlRot = Controller->GetControlRotation();

	FRotator YawRot(0.f, ControlRot.Yaw, 0.f); // Pitch는 제거하고 Yaw만 사용

	FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	// 부동 소수점 관련해서는 IsNearlyZero를 붙이는게 좋음
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(Forward, MoveInput.X);
		// Character 클래스는 이런 AddMovementInput 같은 함수들이 제공됨
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(Right, MoveInput.Y);
	}
}

void AMyCPPCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{	// 함수 내부에 이미 Controller 체크 코드가 있음
		Jump();	// Character 클래스에서 이미 구현되어 있는 기능
	}
}

void AMyCPPCharacter::StopJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{	// 함수 내부에 이미 Controller 체크 코드가 있음
		StopJumping();	// Character 클래스에서 이미 구현되어 있는 기능
	}
}

void AMyCPPCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X); // Character 클래스에서 이미 구현되어 있는 기능
	// Yaw는 좌우 회전

	AddControllerPitchInput(LookInput.Y); // Character 클래스에서 이미 구현되어 있는 기능
	// Pitch는 상하 회전
}

void AMyCPPCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCPPCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}