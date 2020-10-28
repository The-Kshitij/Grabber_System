// Fill out your copyright notice in the Description page of Project Settings.

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
	bool ShouldOpenDoor();

private:
	FRotator Rotation;
	bool IsSoundPlaying=false;
	UAudioComponent *DoorSound=nullptr;
	bool IsDoorOpen=false;
	bool IsDoorClose=true;
	
	UPROPERTY(EditAnywhere);
	ATriggerVolume *PressurePlate;

};
