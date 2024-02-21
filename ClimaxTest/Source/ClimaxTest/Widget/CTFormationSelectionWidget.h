// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Gameplay/CTUnitFormation.h"
#include "CTFormationSelectionWidget.generated.h"

class UCTUnitDirectorComponent;

UCLASS()
class CLIMAXTEST_API UCTFormationSelectionWidget : public UUserWidget
{
	GENERATED_BODY()	

public:
	UFUNCTION(BlueprintCallable, Category = "CT | AI")
	void SetFormationType(EUnitFormationShape InShape);

	UFUNCTION()
	void SetUnitDirectorComponent(UCTUnitDirectorComponent* InDirectorComponent) { DirectorComponent = InDirectorComponent; }

protected:
	UCTUnitDirectorComponent* DirectorComponent;
};
