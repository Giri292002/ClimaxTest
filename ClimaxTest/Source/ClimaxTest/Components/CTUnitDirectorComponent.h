// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTUnitDirectorComponent.generated.h"

class ACTCharacter;
class ACTPlayerController;
class UNiagaraSystem;
class UCTMarqueeWidget;
class ACTHUD;

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

	UFUNCTION()
	void OnMarqueeSelectCallback(TArray<ACTCharacter*> FoundActors);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UCTMarqueeWidget* MarqueeWidget;

	UPROPERTY()
	ACTPlayerController* CTPlayerController;

	UPROPERTY()
	TArray<ACTCharacter*> SelectedUnits;

	UPROPERTY()
	FVector2D StartMousePosition;

	UPROPERTY()
	FVector2D CurrentMousePosition;

	UPROPERTY()
	bool bIsMarqueeSelecting;

	UPROPERTY()
	ACTHUD* CTHUD;
};
