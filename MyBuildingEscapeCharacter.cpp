#include "MyBuildingEscapeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnemyCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

AMyBuildingEscapeCharacter::AMyBuildingEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	StealthTakeDownSceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("Stealth Take Down Attach Component"));
	StealthTakeDownSceneComponent->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


// Input

void AMyBuildingEscapeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMyBuildingEscapeCharacter::Attack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyBuildingEscapeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyBuildingEscapeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyBuildingEscapeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyBuildingEscapeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyBuildingEscapeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyBuildingEscapeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyBuildingEscapeCharacter::OnResetVR);
}

void AMyBuildingEscapeCharacter::Attack() 
{
	if (CanPerformTakedown)
	{
		if (!HasPerformedTakedown)
		{
			//UE_LOG(LogTemp,Warning,TEXT("can perform takedown"));
			AEnemyCharacter *KilledEnemy=Cast<AEnemyCharacter>(HitResult.GetActor());
			if (KilledEnemy)
			{
				if (StealthTakeDownAnimation && BodyMesh)
				{
					
					float Difference=GetCurrentWorldRotation().Yaw-KilledEnemy->GetCurrentWorldRotation().Yaw;
					if (Difference<20 && Difference>=-20)
					{
						KilledEnemy->AttachToComponent(StealthTakeDownSceneComponent,FAttachmentTransformRules::KeepWorldTransform);
						BodyMesh->PlayAnimation(StealthTakeDownAnimation,false);
						UE_LOG(LogTemp,Warning,TEXT("Killed Enemy Location: %s"),*((KilledEnemy->GetCurrentWorldLocation()).ToString()));
						UE_LOG(LogTemp,Warning,TEXT("Main Player Location: %s"),*((GetCurrentWorldLocation()).ToString()));
						UE_LOG(LogTemp,Warning,TEXT("Distance between the two actor : %f"),GetDistanceTo(KilledEnemy));
						if (GetDistanceTo(KilledEnemy)<180)
						{
							UE_LOG(LogTemp,Warning,TEXT("Adding Offset"));
							AddActorWorldOffset(FVector(170,0,0));
						}
						KilledEnemy->RecieveTakeDown();
						HasPerformedTakedown=true;
					}
					else
					{
						UE_LOG(LogTemp,Warning,TEXT("Angle not Enough"));
					}
				}
				else
				{
					UE_LOG(LogTemp,Error,TEXT("BodyMesh or StealthTakeDownAnimation has not been initialised in MyBuildingEscapeCharacter.cpp"));
				}
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("KilledEnemy not initialised"));
			}
		}
	}

}

void AMyBuildingEscapeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyBuildingEscapeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyBuildingEscapeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyBuildingEscapeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyBuildingEscapeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyBuildingEscapeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyBuildingEscapeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyBuildingEscapeCharacter::BeginPlay() 
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("Called BeginPlay"));
	BodyMesh=GetMesh();
	
}

void AMyBuildingEscapeCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	LineStart=GetActorLocation();
	CurrentRotation=GetActorRotation();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FVector LineEnd=LineStart+CurrentRotation.Vector()*170;
	GetWorld()->LineTraceSingleByChannel(HitResult,LineStart,LineEnd,ECollisionChannel::ECC_PhysicsBody,Params);
	DrawDebugLine(GetWorld(),LineStart,LineEnd,FColor::Red);
	if (HitResult.GetActor())
	{
		CanPerformTakedown=true;
	}
	else
	{
		CanPerformTakedown=false;
	}
}
