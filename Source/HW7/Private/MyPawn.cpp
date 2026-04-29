// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "InputActionValue.h"
// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //틱 함수를 사용하기 위해서

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent); // 콜리전 컴포넌트를 루트로 설정

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent); // 메시 컴포넌트를 콜리전 컴포넌트에 붙이는 함수

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CollisionComponent); // 스프링 암 컴포넌트를 콜리전 컴포넌트에 붙이는 함수
	SpringArmComponent->TargetArmLength = 300.0f; // 카메라와의 거리

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent); // 카메라 컴포넌트를 스프링 암 컴포넌트에 붙이는 함수

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetSimulatePhysics(false);

	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = true;

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VerticalVelocity -= GravityScale * DeltaTime; //중력

	FVector LocalMoveDelta = CurrentVelocity * DeltaTime; // 수평 이동량

	float WorldVerticalDelta = VerticalVelocity * DeltaTime; // 수직 이동량

	FHitResult Hit;

	// 수평 이동	
	AddActorLocalOffset(LocalMoveDelta, true, &Hit);

	// 수직 이동은 월드 좌표계에서 처리 (중력과 충돌을 고려하기 위해)
	AddActorWorldOffset(FVector(0.f, 0.f, WorldVerticalDelta), true, &Hit);

	if (Hit.bBlockingHit && WorldVerticalDelta < 0.0f) // 부딪히고, 하강중이라면
	{
		VerticalVelocity = 0.0f; // 땅에 닿았을 때 수직 속도를 0으로 초기화
	}

	if (!CurrentRotationVelocity.IsNearlyZero())
	{
		AddActorLocalRotation(CurrentRotationVelocity * DeltaTime, false);
	}

	else
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = FRotator(0.0f, CurrentRotation.Yaw, 0.0f);
		SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f));
	}

	CurrentVelocity = FVector::ZeroVector;
	CurrentRotationVelocity = FRotator::ZeroRotator;
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMyPawn::Look);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->RollAction, ETriggerEvent::Triggered, this, &AMyPawn::Roll);
			}
			if (PlayerController->LiftAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LiftAction, ETriggerEvent::Triggered, this, &AMyPawn::Lift);
			}
		}
	}

}

void AMyPawn::Move(const FInputActionValue& Value)
{
	// Move 함수 구현
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>(); // 입력값을 2D 벡터로 가져옴

	// InputMapping 에서 X축이 Forward/Backward, Y축이 Right/Left 라고 설정했기 때문에 헷갈리지 않도록 주의

	CurrentVelocity.X = MoveInput.X * MoveSpeed; // X축 입력을 Forward/Backward로 사용
	CurrentVelocity.Y = MoveInput.Y * MoveSpeed; // Y축 입력을 Right/Left로 사용
}
void AMyPawn::Look(const FInputActionValue& Value)
{
	// Look 함수 구현
	if (!Controller) return;

	FVector2D LookInput = Value.Get<FVector2D>(); // 입력값을 2D 벡터로 가져옴

	CurrentRotationVelocity.Yaw = LookInput.X * RotationSpeed;
	CurrentRotationVelocity.Pitch = -LookInput.Y * RotationSpeed; // X축 입력을 Yaw(z축 기준 회전)로, Y축 입력을 Pitch(Y축 기준 회전)로 사용
}
void AMyPawn::Roll(const FInputActionValue& Value)
{
	// Roll 함수 구현
	if (!Controller) return;

	float RollInput = Value.Get<float>(); // 입력값을 float로 가져옴
	CurrentRotationVelocity.Roll = RollInput * RotationSpeed; // Q/E 키 입력을 Roll로 사용
}
void AMyPawn::Lift(const FInputActionValue& Value) // 상승/하강 입력을 처리하는 함수 자연스러운 중력 효고 추가
{
	// Lift 함수 구현
	if (!Controller) return;

	float LiftInput = Value.Get<float>(); // 입력값을 float로 가져옴
	
	if (!FMath::IsNearlyZero(LiftInput))
	{
		VerticalVelocity = LiftInput * MoveSpeed;
	}
}

