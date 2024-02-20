// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTMarqueeWidget.generated.h"

class UBorder;
class UCanvasPanelSlot;

UCLASS()
class CLIMAXTEST_API UCTMarqueeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateMarqueeTransform(FVector2D& MarqueeStart, FVector2D& MarqueeEnd);

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UBorder* MarqueeBorder;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
