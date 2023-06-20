// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretAnimInterface.h"

#define OUT

// Sets default values
ATurrets::ATurrets()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(Root);

	BeamTarget = CreateDefaultSubobject<USceneComponent>(TEXT("BeamTarget"));
	BeamTarget->SetupAttachment(TurretMesh, TEXT("BeamSocket"));

	Target1 = CreateDefaultSubobject<USceneComponent>(TEXT("Target1"));
	Target1->SetupAttachment(Root);

	Target2 = CreateDefaultSubobject<USceneComponent>(TEXT("Target2"));
	Target2->SetupAttachment(Root);

	Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target3"));
	Beam->SetupAttachment(Root);

	FollowTarget = CreateDefaultSubobject<USceneComponent>(TEXT("FollowTarget"));
	FollowTarget->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ATurrets::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ChangeTargetTimerHandle, this, &ATurrets::ChangeProjectileTarget, ChangeTargetDelay, true, 1.f);	
	
	GetWorldTimerManager().SetTimer(TraceTimerHandle, this, &ATurrets::TraceBeam, .1f, true, .1f);

}

// Called every frame
void ATurrets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLookAtTarget(DeltaTime);
	//TraceBeam();
}
/*void ATurrets::UpdateLookAtTarget(float DeltaTime)
{
	if (LookAtRotation.Equals(TargetRotation, 1.f)) 
	{
		return;
	}

	LookAtRotation += RotationDelta * RotationRateMultiplier * DeltaTime;

	/*if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>())
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}

}*/
void ATurrets::ChangeProjectileTarget()
{
	TimerCount++;

	if (TimerCount%2 == 0)
	{
		BeamTarget->SetWorldLocation(Target1->GetComponentLocation());
	}
	else
	{
		BeamTarget->SetWorldLocation(Target2->GetComponentLocation());
	}

}

void ATurrets::UpdateLookAtTarget(float DelaTime)
{
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = BeamTarget->GetComponentLocation();
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>()) 
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ATurrets::SetBeamLength(float Length)
{
	BeamTarget->SetRelativeScale3D(FVector(Length / 400, Beam->GetRelativeScale3D().Y, Beam->GetRelativeScale3D().Z));
	BeamTarget->SetRelativeLocation(FVector(Length / (-8), 0, 0));

}

void ATurrets::TraceBeam()
{
	FHitResult HitResult;
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = Start + BeamTarget->GetForwardVector() * BeamLength;

	FCollisionQueryParams CollQueryParams;
	CollQueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		OUT HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Camera,
		CollQueryParams
	);

	if (bHit)
	{
		SetBeamLength(HitResult.Distance);

	}
	else
	{
		SetBeamLength(BeamLength);
	}
};
