// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Gameplay/CTUnitFormation.h"
#include "CTUnitDirectorComponent.generated.h"

class ACTCharacter;
class ACTPlayerController;
class UNiagaraSystem;
class UCTMarqueeWidget;
class FViewport;
class ACTHUD;
class ACTUnitFormation;
class UCTFormationSelectionWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLIMAXTEST_API UCTUnitDirectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCTUnitDirectorComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CT | FX")
	UNiagaraSystem* ArrowEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CT | FX")
	TSubclassOf<UCTMarqueeWidget> MarqueeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "CT | AI | Formations")
	TSubclassOf<ACTUnitFormation> UnitFormationSubclass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CT | AI | Formations")
	TSubclassOf<UCTFormationSelectionWidget> CTFormationSelectionWidgetClass;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void SelectUnit();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void MarqueeSelectUnits();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void StopMarqueeSelectUnits();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void DeselectUnits();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void MoveUnits();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void ToggleFormationSelectionWidget();

	UFUNCTION(BlueprintCallable, Category = "CT | Units")
	void SetFormationType(EUnitFormationShape InShape) { FormationShape = InShape; }



	UFUNCTION()
	void OnMarqueeSelectCallback(TArray<ACTCharacter*> FoundActors);

	void OnViewportSizeChanged(FViewport* ViewPort, uint32 val);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UCTMarqueeWidget* MarqueeWidget;

	UPROPERTY()
	ACTPlayerController* CTPlayerController;

	UPROPERTY()
	UCTFormationSelectionWidget* FormationSelectionWidget;

	UPROPERTY()
	EUnitFormationShape FormationShape;

	//Using a TSet since I dont care about the order of elements and this is faster than TArray<>.
	UPROPERTY()
	TSet<ACTCharacter*> SelectedUnits;

	UPROPERTY()
	FVector2D StartMousePosition;

	UPROPERTY()
	FVector2D CurrentMousePosition;

	UPROPERTY()
	float ViewportScale;

	UPROPERTY()
	bool bIsMarqueeSelecting;

	UPROPERTY()
	ACTHUD* CTHUD;
};
