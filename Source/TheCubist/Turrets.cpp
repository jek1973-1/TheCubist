// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterInterface.h"
#include "TurretAnimInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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

	Beam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
	Beam->SetupAttachment(TurretMesh, TEXT("BeamSocket"));

	BeamTarget = CreateDefaultSubobject<USceneComponent>(TEXT("BeamTarget"));
	BeamTarget->SetupAttachment(Root);

	Target1 = CreateDefaultSubobject<USceneComponent>(TEXT("Target1"));
	Target1->SetupAttachment(Root);

	Target2 = CreateDefaultSubobject<USceneComponent>(TEXT("Target2"));
	Target2->SetupAttachment(Root);

	FollowTarget = CreateDefaultSubobject<USceneComponent>(TEXT("FollowTarget"));
	FollowTarget->SetupAttachment(Root);	

	SetBeamLength(BeamLength);

	P_MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	P_MuzzleFlash->SetupAttachment(TurretMesh,TEXT("BeamSocket"));
	P_MuzzleFlash->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ATurrets::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ChangeTargetTimerHandle, this, &ATurrets::ChangeBeamTarget, ChangeTargetDelay, true, 1.f);	
	
	GetWorldTimerManager().SetTimer(TraceTimerHandle, this, &ATurrets::TraceBeam, .1f, true, .1f);
}

// Called every frame
void ATurrets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Enemy)
	{
		FollowEnemy(DeltaTime);
	}
	else 
	{
		UpdateLookAtTarget(DeltaTime);
	}
	
	  //TraceBeam();
}

void ATurrets::UpdateLookAtTarget(float DeltaTime)
{
	if (LookAtRotation.Equals(TargetRotation, 1.f))
	{
		return;
	}

	LookAtRotation += RotationDelta * RotationRateMultiplier * DeltaTime;

	if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>())
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ATurrets::ChangeBeamTarget()
{
	TimerCount++;

	if (TimerCount % 2 == 0)
	{
		BeamTarget->SetWorldLocation(Target1->GetComponentLocation());
	}
	else
	{
		BeamTarget->SetWorldLocation(Target2->GetComponentLocation());
	}

	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = BeamTarget->GetComponentLocation();
	TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

	RotationDelta = TargetRotation - LookAtRotation;
	RotationDelta.Normalize();
}

void ATurrets::SetBeamLength(float Length)
{
	Beam->SetRelativeScale3D(FVector(Length / 400, Beam->GetRelativeScale3D().Y, Beam->GetRelativeScale3D().Z));
	Beam->SetRelativeLocation(FVector(Length / (-8), 0, 0));
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
		CheckEnemy(HitResult.GetActor());
	}
	else
	{
		SetBeamLength(BeamLength);
	}
}
void ATurrets::CheckEnemy(AActor* HitActor) 
{
	if (HitActor->Implements<UCharacterInterface>()) 
	{
		bool bEnemy = ICharacterInterface::Execute_IsEnemy(HitActor);
		if (bEnemy)
		{
			if (Enemy)
				return;

			Enemy = HitActor;
			//start shooting
			GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ATurrets::Shoot, .4f, true, .4f);
			//UE_LOG(LogTemp, Warning, TEXT("Enemy Detected"));
		}
	}
	else
	{
		if (Enemy) 
		{
			Enemy = nullptr;
			GetWorldTimerManager().ClearTimer(ShootTimerHandle);
			//stop shooting
		}
		
	}
}

void ATurrets::FollowEnemy(float DeltaTime) 
{
	FVector Start = TurretMesh->GetSocketLocation("BeamSocket");
	FVector End = Enemy->GetActorLocation();

	FRotator RotationToEnemy = UKismetMathLibrary::FindLookAtRotation(Start, End);

	LookAtRotation = FMath::RInterpTo(LookAtRotation, RotationToEnemy, DeltaTime, 100);

	if (TurretMesh->GetAnimInstance()->Implements<UTurretAnimInterface>())
	{
		ITurretAnimInterface::Execute_UpdateLookAtRotation(TurretMesh->GetAnimInstance(), LookAtRotation);
	}
}

void ATurrets::Shoot()
{
	UGameplayStatics::PlaySoundAtLocation(this, ShootSound, P_MuzzleFlash->GetComponentLocation());
	P_MuzzleFlash->Activate(true);

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

	/*if (bHit)
	{
		FPointDamageEvent DamageEvent(10.f, HitResult, BeamTarget->GetForwardVector(), nullptr);
		HitResult.GetActor()->TakeDamage(10.f, DamageEvent, GetInstigatorController(), this);
	}*/
}






