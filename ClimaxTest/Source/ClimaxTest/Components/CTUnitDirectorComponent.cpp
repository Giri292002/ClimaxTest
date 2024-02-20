// Fill out your copyright notice in the Description page of Project Settings.


#include "CTUnitDirectorComponent.h"

#include "../AI/CTAIController.h"
#include "../Core/CTCharacter.h"
#include "../Core/CTPlayerController.h"
#include "../Core/CTHUD.h"
#include "../Widget/CTMarqueeWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values for this component's properties
UCTUnitDirectorComponent::UCTUnitDirectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCTUnitDirectorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CTPlayerController = Cast<ACTPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MarqueeWidget = CreateWidget<UCTMarqueeWidget>(GetWorld(), MarqueeWidgetClass);

	//Unreal has a built in get actors in rectange but the caveat is we can only use it in the draw hud event, so we need to pass on all the info to HUD and get back the selected actors.
	//I am not a huge fan of how this going out of this class and then coming back in with the actor info. If I had more time and this was a proper production setting I would look into decoupling that HUD function from draw HUD
	//and research how we can use it normally. Another method to go about this would be to calculate the 4 points of the box in the world space with the start mouse position and current mouse position and then do a box trace.
	CTHUD = Cast<ACTHUD>(CTPlayerController->GetHUD());
	CTHUD->OnMarqueeSelectDelegate.AddDynamic(this, &UCTUnitDirectorComponent::OnMarqueeSelectCallback);
}

void UCTUnitDirectorComponent::SelectUnit()
{
	//Deselect previously selected units
	DeselectUnits();

	FHitResult HitResult;
	CTPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, HitResult);

	if (HitResult.bBlockingHit)
	{
		if (ACTCharacter* SelectedUnit = Cast<ACTCharacter>(HitResult.GetActor()))
		{
			ICTUnitInterface::Execute_Select(SelectedUnit);
			SelectedUnits.Add(SelectedUnit);
		}
	}
}

void UCTUnitDirectorComponent::MarqueeSelectUnits()
{
	//Store Mouse Starting Location
	bIsMarqueeSelecting = true;
	CTHUD->RunRectSelect(bIsMarqueeSelecting);

	//Viewport mouse position for widgets
	StartMousePosition = CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	FVector2D ScaledPos = StartMousePosition * ViewportScale;
	CTHUD->SetStartMousePosition(ScaledPos);
	CTHUD->SetCurrentMousePosition(ScaledPos);

	//Update Marquee visuals
	MarqueeWidget->AddToViewport();
	MarqueeWidget->UpdateMarqueeTransform(StartMousePosition, CurrentMousePosition);
}

void UCTUnitDirectorComponent::StopMarqueeSelectUnits()
{
	bIsMarqueeSelecting = false;
	CTHUD->RunRectSelect(bIsMarqueeSelecting);
	MarqueeWidget->RemoveFromParent();
}

void UCTUnitDirectorComponent::DeselectUnits()
{
	if (!SelectedUnits.IsEmpty())
	{
		for (auto Unit : SelectedUnits)
		{
			ICTUnitInterface::Execute_Deselect(Unit);
		}
	}
	SelectedUnits.Empty();
}

void UCTUnitDirectorComponent::MoveUnits()
{
	FHitResult HitResult;
	CTPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, HitResult);

	//Try to check if the projected point is on navmesh. If its not on navmesh, the units cant move to it.
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation;
	if (!NavSystem->ProjectPointToNavigation(HitResult.ImpactPoint, ProjectedLocation))
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArrowEffect, ProjectedLocation.Location);

	for (ACTCharacter* Unit : SelectedUnits)
	{
		Cast<ACTAIController>(Unit->GetController())->MoveToLocation(ProjectedLocation);
	}
}

void UCTUnitDirectorComponent::OnMarqueeSelectCallback(TArray<ACTCharacter*> FoundActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Marquee select callback"));
	for (ACTCharacter* Unit : FoundActors)
	{
		ICTUnitInterface::Execute_Select(Unit);
		SelectedUnits.AddUnique(Unit);
	}

	//Deslect units that are not in rectangle anymore
	for (ACTCharacter* Unit : SelectedUnits)
	{
		if (FoundActors.Find(Unit) == -1)
		{
			ICTUnitInterface::Execute_Deselect(Unit);
			SelectedUnits.Remove(Unit);
		}
	}
}


// Called every frame
void UCTUnitDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsMarqueeSelecting)
	{
		//Only run trace and update widget if mouse moved
		FVector2D MouseDelta;
		CTPlayerController->GetInputMouseDelta(MouseDelta.X, MouseDelta.Y);
		bool bCanRunRectSelect = FMath::Abs(MouseDelta.X) > 0.f || FMath::Abs(MouseDelta.Y) > 0.f;
		CTHUD->RunRectSelect(bCanRunRectSelect);
		if (bCanRunRectSelect)
		{	
			CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
			MarqueeWidget->UpdateMarqueeTransform(StartMousePosition, CurrentMousePosition);

			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
			FVector2D ScaledPos = CurrentMousePosition * ViewportScale;
			CTHUD->SetCurrentMousePosition(ScaledPos);
		}
		//GEngine->AddOnScreenDebugMessage(2, 0.2f, FColor::Green, FString::Printf(TEXT("Selecting start %s, end %s"), *StartMousePosition.ToString(), *CurrentMousePosition.ToString()));
	}
}