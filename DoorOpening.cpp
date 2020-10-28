// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpening.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UDoorOpening::UDoorOpening()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDoorOpening::BeginPlay()
{
	Super::BeginPlay();
	Rotation=GetOwner()->GetActorRotation();
	DoorSound=GetOwner()->FindComponentByClass<UAudioComponent>();
	if (DoorSound)
	{
		UE_LOG(LogTemp,Warning,TEXT("DoorSound has been initialized"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("DoorSound has not been initialized for %s"),*(GetOwner()->GetName()));
	}
	
	// ...
}


// Called every frame
void UDoorOpening::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType,ThisTickFunction);
	if (ShouldOpenDoor())
	{
		OpenDoor(DeltaTime);
	}
	else
	{
		CloseDoor(DeltaTime);
	}
	// ...
}
void UDoorOpening::OpenDoor(float DeltaTime)
{
	if (DoorSound)
	{
		if (IsDoorOpen==false && IsDoorClose==true)
		{
			DoorSound->Play();
			IsDoorClose=false;
			IsDoorOpen=true;
		}
	}
	Rotation.Yaw=FMath::Lerp(Rotation.Yaw,0.f,DeltaTime*1);
	GetOwner()->SetActorRotation(Rotation);
}
void UDoorOpening::CloseDoor(float DeltaTime)
{
	if (DoorSound)
	{
		if (IsDoorOpen==true && IsDoorClose==false)
		{
			DoorSound->Play();
			IsDoorOpen=false;
			IsDoorClose=true;
		}
	}
	Rotation.Yaw=FMath::Lerp(Rotation.Yaw,90.f,DeltaTime*2);
	GetOwner()->SetActorRotation(Rotation);
}
bool UDoorOpening::ShouldOpenDoor()
{
	float TotalMass=0.f;
	TArray <UPrimitiveComponent *> OverLappedComponents;
	PressurePlate->GetOverlappingComponents(OverLappedComponents);
	for (UPrimitiveComponent *Component: OverLappedComponents)
	{
		TotalMass+=Component->GetMass();
	}
	if (TotalMass>=1000)
	{
		return true;
	}
	else
	{
		return false;
	}
}