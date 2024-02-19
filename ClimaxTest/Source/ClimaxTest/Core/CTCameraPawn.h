// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CTCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
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

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Input")
	UInputMappingContext* DefaultInputContext;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Defaults")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Defaults")
	float ZoomSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Defaults")
	float MinZoom;

	UPROPERTY(EditDefaultsOnly, Category = "CT | Defaults")
	float MaxZoom;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Zoom(const FInputActionValue& Value);

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
