/*
This is just a dummy to test stealth takedowns
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class MYBUILDINGESCAPE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	
	//This is going to be called by outside objects
	void RecieveTakeDown();

	UFUNCTION(BlueprintImplementableEvent)
	FRotator GetCurrentWorldRotation();

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetCurrentWorldLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	USkeletalMeshComponent *BodyMesh;

	UPROPERTY(EditAnywhere)
	UAnimationAsset *RecieveStealthTakeDownAnimation;

	

};
