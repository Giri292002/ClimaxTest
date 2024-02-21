// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFormationSelectionWidget.h"
#include "../Components/CTUnitDirectorComponent.h"

void UCTFormationSelectionWidget::SetFormationType(EUnitFormationShape InShape)
{
	if (IsValid(DirectorComponent))
	{
		DirectorComponent->SetFormationType(InShape);
	}
}

