// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class HW7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 기본적으로 필요한 컴포넌트들을 선언및 리플랙션 시스템에 등록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 500.0f; // 이동 속도, 블루프린트에서 수정 가능

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 200.0f; // 회전 속도블루프린트에서 수정 가능

	// 블루 프린트에서 안보여도 됨 내부적으로만 사용되는 변수들
	FVector CurrentVelocity = FVector::ZeroVector; // 현재 이동 속도 벡터
	FRotator CurrentRotationVelocity = FRotator::ZeroRotator; // 현재 회전 속도 벡터

	UPROPERTY(EditAnywhere, Category = "Drone|Settings")
	float GravityScale = 300.0f;

	float VerticalVelocity = 0.0f; // 수직 속도	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Roll(const FInputActionValue& Value);
	UFUNCTION()
	void Lift(const FInputActionValue& Value);


};
