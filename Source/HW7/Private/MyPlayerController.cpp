// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
{
	// Input Mapping Context와 Input Action을 블루프린트에서 설정할 수 있도록 UPROPERTY로 선언했음
	InputMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	RollAction = nullptr;
	LiftAction = nullptr;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context와 Input Action이 설정되어 있는지 확인
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // 로컬 플레이어가 존재하는지 확인
	{
		// Enhanced Input 시스템을 사용하여 Input Mapping Context를 추가
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // 우선순위 0으로 추가
			}
		}
	}
}