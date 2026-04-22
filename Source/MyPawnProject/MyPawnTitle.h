#pragma once

#include "CoreMinimal.h"
#include "MyPawnTitle.generated.h"

USTRUCT(BlueprintType)
struct FMyPawnTitle
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title")
	TSet<FString> PawnTitle;

	void AddTitle(const FString& TitleName);

	bool isAchieved(const FString& TitleName);

	void PrintAllTitles();
};