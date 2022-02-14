#include "Grabber_System.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"

// Sets default values for this component's properties
UGrabber_System::UGrabber_System()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber_System::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("Grabber found."));
	
	//Initializations required
	SetInput();
	SetPhysicsHandle();	
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
}

//called when user does the grab actions, bounded in the set input function
void UGrabber_System::Grab()
{
	CalculateLineTraceEnd();
	//only want to grab objects that have physics enabled
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
//called when user does the grab actions, bounded in the set input function
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
		//since the aim to make it work like a grab so using IE_Pressed and IE_Released to ensure that the functions are called when the buttons are pressed and
		// released
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
	LineTraceEnd=PlayerViewPointLocation+PlayerRotation.Vector()*RangeForTrace;
	//adding a custom amount of elevation
	LineTraceEnd.Z=LineTraceEnd.Z-20;
}
