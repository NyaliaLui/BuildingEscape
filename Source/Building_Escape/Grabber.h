// Copyright Nyalia Lui 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach;
		
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY()
	UInputComponent* InputComponent;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	void Grab(void);
	void Drop(void);
	void FindPhysicsHandle(void);
	void SetupInputComponent(void);
	FVector GetPlayerReach(void);
	FHitResult GetFirstPhysicsBodyInReach(void);
};
