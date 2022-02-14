#include "DoorOpening.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Components/PrimitiveComponent.h"


UDoorOpening::UDoorOpening()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoorOpening::BeginPlay()
{
	Super::BeginPlay();
	Rotation=GetOwner()->GetActorRotation();
	//audio component was assigned using the editor
	DoorSound=GetOwner()->FindComponentByClass<UAudioComponent>();
	if (DoorSound)
	{
		UE_LOG(LogTemp,Warning,TEXT("DoorSound has been initialized"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("DoorSound has not been initialized for %s"),*(GetOwner()->GetName()));
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
	/*
	This is to avoid a snapping of the door to the required angle.
	*/
	Rotation.Yaw=FMath::Lerp(Rotation.Yaw,OpenedDoorAngle,DeltaTime*1);
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
	//For smooth movement
	Rotation.Yaw=FMath::Lerp(Rotation.Yaw,ClosedDoorAngle,DeltaTime*2);
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
