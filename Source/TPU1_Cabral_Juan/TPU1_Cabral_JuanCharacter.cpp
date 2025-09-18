// Copyright Epic Games, Inc. All Rights Reserved.
//Incluye el header del personaje, donde se declaran las variables, componentes y funciones
#include "TPU1_Cabral_JuanCharacter.h"

//Permite acceder a información del jugador local, como el control o imputs
#include "Engine/LocalPlayer.h"

//incluye la definición del componente de la cámara, para crearla y manipularla
#include "Camera/CameraComponent.h"

//Incluye la capsula del personaje, que funciona como el collider principal
#include "Components/CapsuleComponent.h"
//maneja toda la lógica de movimiento del personaje:
#include "GameFramework/CharacterMovementComponent.h"
//maneja el brazo de la cámara el SpringArm
#include "GameFramework/SpringArmComponent.h"

//control necesario para obtener input y rotación del personaje
#include "GameFramework/Controller.h"
//bindea imputs a acciones
#include "EnhancedInputComponent.h"

//subsistema que gestiona el imput mapping Context
#include "EnhancedInputSubsystems.h"

//contenedor de los valores de entrada
#include "InputActionValue.h"

//incluye el header del proyecto para obtener las definiciones necesarias del juego
#include "TPU1_Cabral_Juan.h"

ATPU1_Cabral_JuanCharacter::ATPU1_Cabral_JuanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATPU1_Cabral_JuanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Look);
	}
	else
	{
		UE_LOG(LogTPU1_Cabral_Juan, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPU1_Cabral_JuanCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPU1_Cabral_JuanCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}
//función de movimiento de personaje
void ATPU1_Cabral_JuanCharacter::DoMove(float Right, float Forward)
{

	if (GetController() != nullptr)
	{
		//Obtiene la rotación del controller, es decir hacia dónde mira el personaje
		// find out which way is forward
		//YawRotation se usa para movernos en el plano horizontal utilizando los ejes X e Y

		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		//Calcula la dirección hacia adelante
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//calcula la dirección hacia la derecha del jugador.
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		//aplica el movimiento al Character
		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
//implementacion de la función mirar

void ATPU1_Cabral_JuanCharacter::DoLook(float Yaw, float Pitch)
{ 
	//Comprueba que el personaje tiene un controlador asignado
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		//Aplica rotación horizontal al controller del personaje, modificando el Yaw de la cámara.
		AddControllerYawInput(Yaw);
		//Aplica rotación vertical al controller que modifica el Pitch de la cámara.
		AddControllerPitchInput(Pitch);
	}
}
//funciones callback para manejar el salto y el final de la accion de salto
void ATPU1_Cabral_JuanCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ATPU1_Cabral_JuanCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
