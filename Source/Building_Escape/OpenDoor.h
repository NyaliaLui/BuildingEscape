// Copyright Nyalia Lui 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors(void);

private:
	float InitialYaw;
	float CurrentYaw;
	
	UPROPERTY(EditAnywhere)
	float TargetYaw;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	float DoorLastOpened;
	float DoorClosedDelay;

	UPROPERTY(EditAnywhere)
	float DoorSpeed;

	UPROPERTY(EditAnywhere)
	float MassRequirement;

	UPROPERTY()
	UAudioComponent *AudioComponent;
	bool OpenDoorSound;
	bool CloseDoorSound;

	void FindPressurePlate(void);
	void SetupAudioComponent(void);
};
