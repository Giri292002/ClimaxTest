// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTHUD.generated.h"

class ACTCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMarqueeSelectSignature, TArray<ACTCharacter*>, FoundActors);
UCLASS()
class CLIMAXTEST_API ACTHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMarqueeSelectSignature OnMarqueeSelectDelegate;

	UFUNCTION()
	void RunRectSelect(const bool InbIsDrawing) { bRunRectTrace = InbIsDrawing; }

	UFUNCTION()
	void SetStartMousePosition(FVector2D& InStartMousePosition) { StartMousePosition = InStartMousePosition; }

	UFUNCTION()
	void SetCurrentMousePosition(FVector2D& InCurrentMousePosition) { CurrentMousePosition = InCurrentMousePosition; }

	UFUNCTION()
	FVector2D GetCurrentMousePosition() const { return CurrentMousePosition; }

	UFUNCTION()
	FVector2D GetStartMousePosition() const { return StartMousePosition; }

	virtual void DrawHUD() override;	

protected:
	UPROPERTY()
	FVector2D StartMousePosition;

	UPROPERTY()
	FVector2D CurrentMousePosition;

	UPROPERTY()
	bool bRunRectTrace;

	UPROPERTY()
	TArray<ACTCharacter*> FoundUnits;

};
