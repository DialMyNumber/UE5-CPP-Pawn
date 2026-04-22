#pragma once

#include "CoreMinimal.h"
#include "MyInventory.generated.h"

USTRUCT(BlueprintType)
struct FMyInventory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FString> InventoryArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FString, int32> InventoryMap;

	void AddItem(const FString& ItemName);

	void RemoveItem(const FString& ItemName);

	void ShowInventoryArr();

	void ShowInventoryMap();
};
