#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBuildingEscapeCharacter.generated.h"

UCLASS(config=Game)
class AMyBuildingEscapeCharacter : public ACharacter
{
	GENERATED_BODY()

	/*
	* Camera boom, positioning the camera behind the character 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* 
	*Follow camera 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyBuildingEscapeCharacter();

	/*
	* Base turn rate, in deg/sec. Other scaling may affect final turn rate. 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/*
	* Base look up/down rate, in deg/sec. Other scaling may affect final rate. 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/*
	* Resets HMD orientation in VR. 
	*/
	void OnResetVR();

	/*
	* Called for forwards/backward input 
	*/
	void MoveForward(float Value);

	/*
	* Called for side to side input 
	*/
	void MoveRight(float Value);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
	FRotator GetCurrentWorldRotation();

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetCurrentWorldLocation();

private:
	bool CanPerformTakedown=false;
	bool HasPerformedTakedown=false;

	USkeletalMeshComponent *BodyMesh;
	
	//to store the result of a line trace
	FHitResult HitResult;
	
	FVector LineStart;
	FRotator CurrentRotation;

	class AEnemyCharacter *EnemyPlayer;

	void Attack();

	UPROPERTY(EditAnywhere)
	UAnimationAsset *StealthTakeDownAnimation;

	UPROPERTY(EditAnywhere)
	USceneComponent *StealthTakeDownSceneComponent;

};

