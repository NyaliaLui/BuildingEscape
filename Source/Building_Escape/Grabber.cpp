// Copyright Nyalia Lui 2020


#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
	:Reach(200.0f),
	PhysicsHandle(nullptr),
	InputComponent(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	this->FindPhysicsHandle();
	this->SetupInputComponent();	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move the object we're holding if the physics handle is attached
	if (this->PhysicsHandle && this->PhysicsHandle->GetGrabbedComponent()) {
		this->PhysicsHandle->SetTargetLocation(this->GetPlayerReach());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("UGrabber::TickComponent has nullptr PhysicsHandle"));
	}
}

void UGrabber::FindPhysicsHandle(void)
{
	this->PhysicsHandle = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!this->PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("UGrabber::FindPhysicsHandle Physics Handle wasn't initialized on %s!"), *this->GetOwner()->GetName());
		return;
	}
}

void UGrabber::SetupInputComponent(void)
{
	this->InputComponent = this->GetOwner()->FindComponentByClass<UInputComponent>();

	if (this->InputComponent) {
		this->InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		this->InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
	} else {
		UE_LOG(LogTemp, Error, TEXT("UGrabber::SetupInputComponent Input component not found on %s!"), *this->GetOwner()->GetName());
	}
}

void UGrabber::Grab(void)
{
	if (!this->PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("UGrabber::Grab Physics Handle wasn't initialized on %s!"), *this->GetOwner()->GetName());
		return;
	}
	FVector LineTraceEnd = this->GetPlayerReach();

	// Only ray cast when key is pressed and see if we reach
	FHitResult Hit = this->GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = Hit.GetComponent();

	// Try and reach any actors with physics body collision channel set.

	// If we hit something then attach the physics handle.
	if (Hit.GetActor()) {
		this->PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Drop(void)
{
	if (!this->PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("UGrabber::Drop Physics Handle wasn't initialized on %s!"), *this->GetOwner()->GetName());
		return;
	}

	this->PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetPlayerReach(void)
{
	this->GetWorld()->GetFirstPlayerController()
	->GetPlayerViewPoint(this->PlayerViewPointLocation, this->PlayerViewPointRotation);

	FVector LineTraceEnd = this->PlayerViewPointLocation + this->PlayerViewPointRotation.Vector() * this->Reach;
	return LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach(void)
{
	// Get players viewpoint
	FVector PlayerReach = this->GetPlayerReach();

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this->GetOwner());
	this->GetWorld()->LineTraceSingleByObjectType(
		Hit,
		this->PlayerViewPointLocation,
		PlayerReach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

