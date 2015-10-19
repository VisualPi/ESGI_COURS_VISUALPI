// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlankProj.h"
#include "Case.h"


// Sets default values
ACase::ACase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

