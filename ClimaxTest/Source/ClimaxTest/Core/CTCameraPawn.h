// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CTCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UCTCameraPawnData;
class UCTUnitDirectorComponent;
struct FInputActionValue;

UCLASS()
class CLIMAXTEST_API ACTCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACTCameraPawn();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CT | Components")
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Components")
	UCTUnitDirectorComponent* UnitDirectorComponent;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* MoveCameraAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* ZoomCameraAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* MoveUnitAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* SelectUnitAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* MarqueeSelectUnitAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* FormationSelectWidgetAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputMappingContext* DefaultInputContext;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Data")
	const UCTCameraPawnData* CameraPawnData;

	UFUNCTION()
	void MoveCamera(const FInputActionValue& Value);

	UFUNCTION()
	void ZoomCamera(const FInputActionValue& Value);

	UFUNCTION()
	void EdgeScroll();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	FVector CameraTargetLocation;

	UPROPERTY()
	float TargetZoom;
};
