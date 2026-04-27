#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class MYPAWNPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp; // Static Mesh ÄÄÆ÷³ÍÆ®

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	virtual void BeginPlay() override;

	FVector Velocity;

	float MoveAccel;
	float VerticalAccel;
	float Drag;
	float MaxSpeed;

	float YawSpeed;
	float CurrentYawInput;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PMove(const FInputActionValue& value);

	UFUNCTION()
	void PUpDown(const FInputActionValue& value);

	UFUNCTION()
	void PYaw(const FInputActionValue& value);

	UFUNCTION()
	void PLook(const FInputActionValue& value);

};
