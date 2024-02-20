// Fill out your copyright notice in the Description page of Project Settings.


#include "CTHUD.h"
#include "CTCharacter.h"

void ACTHUD::DrawHUD()
{
	Super::DrawHUD();
	if (bIsMarqueeSelecting)
	{
		FoundUnits.Empty();	
		if (GetActorsInSelectionRectangle<ACTCharacter>(StartMousePosition, CurrentMousePosition, FoundUnits))
		{
			if (!FoundUnits.IsEmpty())
			{
				OnMarqueeSelectDelegate.Broadcast(FoundUnits);
			}
		}
	}
}
