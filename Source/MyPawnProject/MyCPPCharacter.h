#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyInventory.h"
#include "MyPawnTitle.h"
#include "MyCPPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MYPAWNPROJECT_API AMyCPPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCPPCharacter();

	// VisibleAnywhere : 세부 속성은 조절 불가능하지만 내부적으로 기본으로 존재하는 속성들은 수정 가능
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FMyInventory PlayerInventory;

	FMyPawnTitle PlayerTitle;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
