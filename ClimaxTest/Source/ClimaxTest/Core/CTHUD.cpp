// Fill out your copyright notice in the Description page of Project Settings.


#include "CTHUD.h"
#include "CTCharacter.h"

ACTHUD::ACTHUD()
{
	PreviousMousePosition = FVector2D::Zero();
}

void ACTHUD::DrawHUD()
{
	Super::DrawHUD();
	if (bRunRectTrace)
	{
		if (!CurrentMousePosition.Equals(PreviousMousePosition, 1.f))
		{
			PreviousMousePosition = CurrentMousePosition;
			FoundUnits.Empty();
			if (GetActorsInSelectionRectangle<ACTCharacter>(StartMousePosition, CurrentMousePosition, FoundUnits, false))
			{
				OnMarqueeSelectDelegate.Broadcast(FoundUnits);
			}
		}
	}
}
