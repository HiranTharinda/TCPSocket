// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h>
#include "SocketServer.generated.h"

UCLASS()
class TCPBUILDER_API ASocketServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASocketServer();
	~ASocketServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool Connect(FString ip, int32 port);
	void TCPSocketListener();
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	FSocket* TCPSocket;
	FInternetAddr* addr;
	bool connected;
	
};
