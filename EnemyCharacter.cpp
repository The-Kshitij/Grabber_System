// Fill out your copyright notice in the Description page of Project Settings
#include "EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCharacter::RecieveTakeDown() 
{
	if (BodyMesh && RecieveStealthTakeDownAnimation)
	{
		BodyMesh->PlayAnimation(RecieveStealthTakeDownAnimation,false);
		UE_LOG(LogTemp,Warning,TEXT("Recieved Takedown"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("BodyMesh or RecieveStealthTakeDownAnimation has not been initialised in EnemyCharacter.cpp"));
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	BodyMesh=GetMesh();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}