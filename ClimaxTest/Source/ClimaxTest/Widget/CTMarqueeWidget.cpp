// Fill out your copyright notice in the Description page of Project Settings.


#include "CTMarqueeWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

void UCTMarqueeWidget::UpdateMarqueeTransform(FVector2D& MarqueeStart, FVector2D& MarqueeEnd)
{
	//We always want the anchor to be top left no mattter where the player has pulled it from
	const FVector2D AnchorPosition = FVector2D(FMath::Min(MarqueeStart.X, MarqueeEnd.X), FMath::Min(MarqueeStart.Y, MarqueeEnd.Y));
	const FVector2D MarqueeSize = FVector2D(FMath::Abs(MarqueeStart.X - MarqueeEnd.X), FMath::Abs(MarqueeStart.Y - MarqueeEnd.Y));

	UWidgetLayoutLibrary::SlotAsCanvasSlot(MarqueeBorder)->SetPosition(AnchorPosition);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(MarqueeBorder)->SetSize(MarqueeSize);
}

void UCTMarqueeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}
