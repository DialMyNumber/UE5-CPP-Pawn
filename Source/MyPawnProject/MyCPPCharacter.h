#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCPPCharacter.generated.h"

UCLASS()
class MYPAWNPROJECT_API AMyCPPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCPPCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
