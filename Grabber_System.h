/*
If the player holds down on a button then he/she should be able to grab the object and when the player releases the button then the object should be released.
Line traces are done to check for the range uptill which a player can grab an object.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber_System.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYBUILDINGESCAPE_API UGrabber_System : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber_System();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab();
	void Release();
	//Assigns the physics handle
	void SetPhysicsHandle();
	//As the name suggests, it does the required bindings for players input
	void SetInput();
	//
	void CalculateLineTraceEnd();

private:
	//variable used for the maximum distance upto which the player can grab an object
	UPROPERTY(EditAnywhere)
	float RangeForTrace = 400.0f;
	
	FVector PlayerViewPointLocation;
	FRotator PlayerRotation;
	FVector LineTraceEnd;
	UPhysicsHandleComponent *PhysicsHandle=nullptr;
	UPrimitiveComponent *ComponentGrabbed=nullptr;
	UInputComponent *InputComponent=nullptr;
	FHitResult HitResult;
	bool GrabState=false;
};
