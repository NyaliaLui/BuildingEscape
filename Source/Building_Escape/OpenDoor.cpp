// Copyright Nyalia Lui 2020

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
	:InitialYaw(0.0f),
	CurrentYaw(0.0f),
	TargetYaw(90.0f),
	PressurePlate(nullptr),
	DoorLastOpened(-1.0f),
	DoorClosedDelay(1.5f),
	DoorSpeed(1.0f),
	MassRequirement(50.0f),
	AudioComponent(nullptr),
	OpenDoorSound(false),
	CloseDoorSound(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	this->InitialYaw = this->GetOwner()->GetActorRotation().Yaw;
	this->CurrentYaw = this->InitialYaw;
	this->TargetYaw += this->InitialYaw;

	this->FindPressurePlate();
	this->SetupAudioComponent();
	this->OpenDoorSound = false;
	this->CloseDoorSound = true;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (this->TotalMassOfActors() >= this->MassRequirement) {
		this->OpenDoor(DeltaTime);
		this->DoorLastOpened = this->GetWorld()->GetTimeSeconds();
	} else {
		if ((this->GetWorld()->GetTimeSeconds() - this->DoorLastOpened) >= this->DoorClosedDelay) {
			this->CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	this->CurrentYaw = FMath::Lerp(this->CurrentYaw, this->TargetYaw, DeltaTime * this->DoorSpeed);
	FRotator DoorRotation = this->GetOwner()->GetActorRotation();
	DoorRotation.Yaw = this->CurrentYaw;
	this->GetOwner()->SetActorRotation(DoorRotation);
	this->CloseDoorSound = false;

	if (this->AudioComponent && !this->OpenDoorSound) {
		this->AudioComponent->Play();
		this->OpenDoorSound = true;
	} else {
		UE_LOG(LogTemp, Error, TEXT("UOpenDoor::OpenDoor Audio wasn't initialized on %s!"), *this->GetOwner()->GetName());
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	this->CurrentYaw = FMath::Lerp(this->CurrentYaw, this->InitialYaw, DeltaTime * this->DoorSpeed);
	FRotator DoorRotation = this->GetOwner()->GetActorRotation();
	DoorRotation.Yaw = this->CurrentYaw;
	this->GetOwner()->SetActorRotation(DoorRotation);
	this->OpenDoorSound = false;

	if (this->AudioComponent && !this->CloseDoorSound) {
		this->AudioComponent->Play();
		this->CloseDoorSound = true;
	} else {
		UE_LOG(LogTemp, Error, TEXT("UOpenDoor::CloseDoor Audio wasn't initialized on %s!"), *this->GetOwner()->GetName());
	}
}

float UOpenDoor::TotalMassOfActors(void)
{

	if (!this->PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("UOpenDoor::TotalMassOfActors PressurePlate is nullptr on %s!"), *this->GetOwner()->GetName());
		return 0.0f;
	}

	float TotalMass = 0.0f;

	// Find all overlapping actors
	TArray<UPrimitiveComponent*> OverlappingComponents;
	this->PressurePlate->GetOverlappingComponents(OUT OverlappingComponents);

	// Sum their mass
	for (const UPrimitiveComponent* prim : OverlappingComponents) {
		TotalMass += prim->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindPressurePlate(void)
{
	if (!this->PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("UOpenDoor::BeginPlay PressurePlate is nullptr on %s!"), *this->GetOwner()->GetName());
	}
}

void UOpenDoor::SetupAudioComponent(void) {
	this->AudioComponent = this->GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!this->AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("UGrabber::SetupAudioComponent Audio wasn't initialized on %s!"), *this->GetOwner()->GetName());
		return;
	}
}