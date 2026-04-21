#include "MyCPPCharacter.h"

// Sets default values
AMyCPPCharacter::AMyCPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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

