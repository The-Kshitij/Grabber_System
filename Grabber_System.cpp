// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber_System.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"

// Sets default values for this component's properties
UGrabber_System::UGrabber_System()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber_System::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("Grabber has been compiled."));
	SetInput();
	SetPhysicsHandle();
	// ...
	
}


// Called every frame
void UGrabber_System::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GrabState==true)
	{
		UE_LOG(LogTemp,Warning,TEXT("Entering GrabState in TickComponent"));
		CalculateLineTraceEnd();
		PhysicsHandle->SetTargetLocation(LineTraceEnd+FVector(0,0,30));
	}
	// ...
}
void UGrabber_System::HitDetection()
{
}
void UGrabber_System::Grab()
{
	CalculateLineTraceEnd();
	FCollisionObjectQueryParams CollisionObjects(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams AdditionalParams(FName(TEXT("")),false,GetOwner());
	if (PhysicsHandle)
	{
		if (GrabState==false)
		{
			//HitResult has been initialized in Grabber_System.h
			GetWorld()->LineTraceSingleByObjectType(HitResult,PlayerViewPointLocation,LineTraceEnd,CollisionObjects,AdditionalParams);
			if (HitResult.GetComponent())
			{
				UE_LOG(LogTemp,Warning,TEXT("Ray Has Hit: %s"),*(HitResult.GetActor()->GetName()));
				ComponentGrabbed=HitResult.GetComponent();
				PhysicsHandle->GrabComponent(ComponentGrabbed,NAME_None,LineTraceEnd,false);
				GrabState=true;
			}
		}
	}
}
void UGrabber_System::Release()
{
	PhysicsHandle->ReleaseComponent();
	GrabState=false;
}
void UGrabber_System::SetPhysicsHandle()
{
	PhysicsHandle=GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp,Warning,TEXT("PhysicsHandle found on %s"),*(GetOwner()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("No PhysicsHandle on %s"),*(GetOwner()->GetName()));
	}
}
void UGrabber_System::SetInput()
{
	InputComponent=GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("GrabAction",IE_Pressed,this,&UGrabber_System::Grab);
		InputComponent->BindAction("GrabAction",IE_Released,this,&UGrabber_System::Release);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("No Input Component found on %s"),*(GetOwner()->GetName()));
	}
}
void UGrabber_System::CalculateLineTraceEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation,PlayerRotation);
	LineTraceEnd=PlayerViewPointLocation+PlayerRotation.Vector()*400;
	LineTraceEnd.Z=LineTraceEnd.Z-20;
}