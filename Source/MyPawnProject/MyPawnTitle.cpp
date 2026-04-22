#include "MyPawnTitle.h"

void FMyPawnTitle::AddTitle(const FString& TitleName)
{
	PawnTitle.Add(TitleName);
}

bool FMyPawnTitle::isAchieved(const FString& TitleName)
{
	return PawnTitle.Contains(TitleName);
}

void FMyPawnTitle::PrintAllTitles()
{
	for (const FString& title : PawnTitle) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *title);
	}
}