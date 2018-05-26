// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject <UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(OverlapComp);

}



void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Null Pawn"))

		return;
	}

	if (MyPawn->bIsCarryingObjective)
	{
		UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"))
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (EnterSound) {
			UGameplayStatics::PlaySound2D(this, EnterSound);
		}
		
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NO OBJECTIVE"))

		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}



}



