// Copyright Epic Games, Inc. All Rights Reserved.
//Evita que el archivo .h se incluya más de una vez durante la compilación, evitando definiciones múltiples.
#pragma once
//Incluye el set básico de tipos y definiciones fundamentales de Unreal. Ahorra tiempo en la compilaciín porque ya este "pre-compilado".
#include "CoreMinimal.h"
//Le dice al compilador que incluya el archivo donde está definida la clase ACharacter.
#include "GameFramework/Character.h"

//Conjunto de macros en C++ que permiten escribir mensajes en la consola del editor y del juego en ejecución.
#include "Logging/LogMacros.h"

// Se utiliza para incluir el código generado automáticamente por el sistema de reflexión del motor. Ayuda en la comunicación entre c++ y blueprints.
// Siempre tiene que ir al final de los #include en el header.
#include "TPU1_Cabral_JuanCharacter.generated.h"


//Los class le dice al compilador que existe una clase llamada de esa forma
//el springarm sirve para ubicar la camara a una distancia del character 
class USpringArmComponent;
//es el componente de la camara
class UCameraComponent;
//representa el sistema de entrada de acciones asignados a teclas o botones
class UInputAction;

//Struct: estructuras de datos que organizan propiedades 
//guarda el valor asociado a una entrada que representa una acción.
struct FInputActionValue;

//sirve para declarar una categoría de registro externa, llamada LogTemplateCharacter, para que c++ la pueda usar
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
//Establece una clase base con una estructura y funcionalidades esperadas y que todas las clases que hereden de ella lva a incluirlas. 
UCLASS(abstract)

//describe la estructura de la herencia y que ACharacter es clase base
class ATPU1_Cabral_JuanCharacter : public ACharacter
{
	//macro para configurar la clase
	GENERATED_BODY()
	
	//Sirve para colocar la cámara detrás del personaje
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	//Sirve para que la cámara siga al personaje

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	//Listado de referencias de imput actions dentro del personaje
	//accion de salto
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;
	
	//accion de movimiento
	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	//Acción de mirar
	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	
	//Acción de mirar específica para el mouse
	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor del  personaje */
	ATPU1_Cabral_JuanCharacter();	

protected:
	//conectar imputs con funciones de c++
	//Sobreescribe la función de ACharacter.
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//funciones callbacks para los imputs
	//FInputActionValue actua como contenedor de datos que interpreta para traducir los valores en movimiento
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	//procesa el valor de rotacion para realizar la acción
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	//Registra valores de entrada para movimiento del personaje.
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	//Registra valores de entrada para movimiento de mirar o girar la cámara.
	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);
	
	//Maneja el inicio de un salto (cuando se presiona la tecla).
	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	//Maneja el fin del salto.
	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	//el FORCEINLINE le dice al compilador que expanda la función directamente en lugar de hacer una llamada, es decir que inserta el código de la función
	//Permite acceder al componente desde otras clases o desde Blueprints, en este caso sobre referencias de la cámara yseguimiento de camara
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

