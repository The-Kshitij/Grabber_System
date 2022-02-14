#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "DoorOpening.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYBUILDINGESCAPE_API UDoorOpening : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpening();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	
	/*
	The goal of the game was to grab and place objects on the pressure plate so that the door would open. The below function checks if the 
	weight of the objects on the pressure plate is sufficient for opening the door or not.
	*/
	bool ShouldOpenDoor();

private:
	//The two variable below are to make the door generic, different doors could have different opening and closing positions
	UPROPERTY(EditAnywhere)
	float OpenedDoorAngle = 0.f;
	UPROPERTY(EditAnywhere)
	float ClosedDoorAngle = 0.f;
	/*
	The door has a pressure plate, if you have sufficient weight on the pressure plate, only then would the door open. 
	*/
	//Used for storing the rotation of the door
	FRotator Rotation;
	UAudioComponent *DoorSound=nullptr;
	
	//by default the door is closed
	bool IsDoorOpen=false;
	bool IsDoorClose=true;
	
	//reference to pressure plate, assigned in the editor
	UPROPERTY(EditAnywhere);
	ATriggerVolume *PressurePlate;

};
