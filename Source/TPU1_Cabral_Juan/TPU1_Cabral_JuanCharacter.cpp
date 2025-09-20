// Copyright Epic Games, Inc. All Rights Reserved.
//Incluye el header del personaje, donde se declaran las variables, componentes y funciones
#include "TPU1_Cabral_JuanCharacter.h"

//Permite acceder a informaci�n del jugador local, como el control o imputs
#include "Engine/LocalPlayer.h"

//incluye la definici�n del componente de la c�mara, para crearla y manipularla
#include "Camera/CameraComponent.h"

//Incluye la capsula del personaje, que funciona como el collider principal
#include "Components/CapsuleComponent.h"
//maneja toda la l�gica de movimiento del personaje:
#include "GameFramework/CharacterMovementComponent.h"
//maneja el brazo de la c�mara el SpringArm
#include "GameFramework/SpringArmComponent.h"

//control necesario para obtener input y rotaci�n del personaje
#include "GameFramework/Controller.h"
//bindea imputs a acciones
#include "EnhancedInputComponent.h"

//subsistema que gestiona el imput mapping Context
#include "EnhancedInputSubsystems.h"

//contenedor de los valores de entrada
#include "InputActionValue.h"

//incluye el header del proyecto para obtener las definiciones necesarias del juego
#include "TPU1_Cabral_Juan.h"

//se cambiaron todos los par�metros .f para probar 
// establece la clase base de la cual van a herecdar los child
ATPU1_Cabral_JuanCharacter::ATPU1_Cabral_JuanCharacter()
{
	// Set size for collision capsule
	// establece el tama�o de la c�psula del character.
	GetCapsuleComponent()->InitCapsuleSize(65.f, 76.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	//desactiva la rotaci�n de un personaje en los ejes de pitch, yaw y roll y que solo se mueva la c�mara.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	//configura el comportamiento de movimiento de un personaje para que siempre mire en la direcci�n en la que se mueve.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	//establece valores de distint�s parametros del character
	// velocidad inicial en el eje Z al saltar. 
	GetCharacterMovement()->JumpZVelocity = 620.f;
	//el control del personaje en el aire cuando est� en el aire
	GetCharacterMovement()->AirControl = 0.95f;
	//maxima velocidad para caminar
	GetCharacterMovement()->MaxWalkSpeed = 740.f;
	//velocidad minima para para cominar
	GetCharacterMovement()->MinAnalogWalkSpeed = 200.f;
	//que tan r�pido el personaje desacelera y se detiene cuando camina
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	//que tan r�pido el personaje desacelera cuando est� cayendo
	GetCharacterMovement()->BrakingDecelerationFalling = 500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	//configuran el SpringArm de la cam�ra

	//crea el SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//se adjunta al RootComponent del personaje
	CameraBoom->SetupAttachment(RootComponent);
	//maneja la distancia
	CameraBoom->TargetArmLength = 900.0f;
	//para tener control de giro de la c�mara
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	//configuracion de la camara, la crea, asigna como su root al sprinarm y evita que rote la camara junto al personaje
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}
//configura las acciones de input del jugador, asignamos la acci�n correspondiente segun la tecla de input
void ATPU1_Cabral_JuanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings ( vincula una acci�n (input) a una funci�n de c�digo.)
	//comprueba el sistema de entrada, el Enhanced Input System
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		//funacion de salto, empieza cuando apreto y termina cuando suelto la tecla
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// nos movemos continuamente mientras apretemos la tecla
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Move);
		// conecta el movimiento del mouse con la rotaci�n de la c�mara en el juego,
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Look);

		// Looking
		
		// bindea la acci�n de mirar a la funci�n de c�digo que maneja la rotaci�n de la c�mara
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::Look);
	
	    //dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ATPU1_Cabral_JuanCharacter::dashing);

		//bindeamos el inicio y fin de la acci�n correr segun empezamos y soltamos la tecla
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ATPU1_Cabral_JuanCharacter::StartCorrer);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ATPU1_Cabral_JuanCharacter::Endcorrer);
		// mostrar un mensaje cuando apretamos la tecla
		EnhancedInputComponent->BindAction(MostrarMensajeAction, ETriggerEvent::Triggered, this, &ATPU1_Cabral_JuanCharacter::MostrarMensaje);

	}
	else
	{//cuando hay un error al encontrar el enhanced imput component muestra ese mensaje de error del text
		UE_LOG(LogTPU1_Cabral_Juan, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//maneja el movimiento del personaje en base a los valores de entrada
void ATPU1_Cabral_JuanCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	//Calcula el vector de direcci�n de movimiento segun la entrada
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	//toma el vector y lo traduce en movimiento
	DoMove(MovementVector.X, MovementVector.Y);
}
//maneja la rotaci�n de la c�mara del personaje en base a valores de entrada
void ATPU1_Cabral_JuanCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	//extrae la entrada del jugador para la rotaci�n de la c�mara y la almacena en un vector 2D.
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	//toma el vector para establecer a donde miramos
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}
//Funcion implementada correr, aumenta la velocidad de caminar en StartCorrer y vuelve a la velocidad inicial en el Endcorrer
void ATPU1_Cabral_JuanCharacter::StartCorrer()
{
	GetCharacterMovement()->MaxWalkSpeed = 1800.f;
}

void ATPU1_Cabral_JuanCharacter::Endcorrer()
{
	GetCharacterMovement()->MaxWalkSpeed = 740.f;

}


void ATPU1_Cabral_JuanCharacter::dashing()
{
	// Obtener la direcci�n del movimiento actual del personaje y si no se mueve hacer el dash hacia donde est� mirando
	FVector DashDirection = GetVelocity().GetSafeNormal();
	if (DashDirection.IsNearlyZero())
	{
		DashDirection = GetActorForwardVector();
	}

	// Llamo a la funci�n LaunchCharacter en el personaje.
	LaunchCharacter(DashDirection * 12000.f, true, true);
}

//el contenido de la funci�n mostrar mensaje con la duraci�n en pantalla, color y mensaje
void ATPU1_Cabral_JuanCharacter::MostrarMensaje()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("Hola Kent, Mejorate pronto!!")));
	}
}

//funci�n de movimiento de personaje
void ATPU1_Cabral_JuanCharacter::DoMove(float Right, float Forward)
{

	if (GetController() != nullptr)
	{
		//Obtiene la rotaci�n del controller, es decir hacia d�nde mira el personaje
		// find out which way is forward
		//YawRotation se usa para movernos en el plano horizontal utilizando los ejes X e Y

		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		//Calcula la direcci�n hacia adelante
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//calcula la direcci�n hacia la derecha del jugador.
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		//aplica el movimiento al Character
		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
//implementacion de la funci�n mirar

void ATPU1_Cabral_JuanCharacter::DoLook(float Yaw, float Pitch)
{ 
	//Comprueba que el personaje tiene un controlador asignado
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		//Aplica rotaci�n horizontal al controller del personaje, modificando el Yaw de la c�mara.
		AddControllerYawInput(Yaw);
		//Aplica rotaci�n vertical al controller que modifica el Pitch de la c�mara.
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

