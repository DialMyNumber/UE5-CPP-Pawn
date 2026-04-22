#include "MyInventory.h"

void FMyInventory::AddItem(const FString& ItemName)
{
	InventoryArr.Add(ItemName);

	int32& Count = InventoryMap.FindOrAdd(ItemName);	// Map에 추가, Key = FString ItemName
	Count++;
}

void FMyInventory::RemoveItem(const FString& ItemName)
{
	if (InventoryArr.RemoveSingle(ItemName) > 0) // 하나만 제거
	{
		int32* CountPtr = InventoryMap.Find(ItemName);

		if (CountPtr)
		{
			(*CountPtr)--;

			if (*CountPtr <= 0)
			{
				InventoryMap.Remove(ItemName);
			}
		}
	}
}

void FMyInventory::ShowInventoryArr()
{
	for (const FString& Item : InventoryArr)
	{
		UE_LOG(LogTemp, Log, TEXT("Item: %s"), *Item);
	}
}

void FMyInventory::ShowInventoryMap()
{
	for (const auto& item : InventoryMap)
	{
		UE_LOG(LogTemp, Log, TEXT("Item: %s, Count: %d"), *item.Key, item.Value);
		// UE_LOG의 %s에 FString을 사용하려면 역참조 사용
	}
}