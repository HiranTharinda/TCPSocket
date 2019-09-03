// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketServer.h"
#include <Runtime/Core/Public/Containers/Ticker.h>
#include <SocketSubsystem.h>
#include <IPAddress.h>
#include <Sockets.h>
#include <Runtime\Networking\Public\Common\TcpSocketBuilder.h>
#include <string>

// Sets default values
ASocketServer::ASocketServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ASocketServer::~ASocketServer()
{
	
}


bool ASocketServer::Connect(FString ip, int32 port)
{
	UE_LOG(LogTemp, Warning, TEXT("Initialized"));

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address address;
	FIPv4Address::Parse(ip, address);
	addr->SetIp(address.Value);
	addr->SetPort(port);

	TCPSocket = FTcpSocketBuilder("ImageSocket")
		.AsNonBlocking()
		.AsReusable()
		.BoundToAddress(address.Value)
		.BoundToPort(port)
		.WithReceiveBufferSize(1024 * 1024);


	if (TCPSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket Created! Connecting to server"));
		connected = TCPSocket->Connect(*addr);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't Create Socket"));
		return(false);
	}
	if (connected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected"));
		return(true);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Connected"));
		return(false);
	}
	return(false);
}



void ASocketServer::TCPSocketListener()
{
	//~~~~~~~~~~~~~
	if (!TCPSocket) return;
	//~~~~~~~~~~~~~
	uint32 Size = 1024;
	UE_LOG(LogTemp, Warning, TEXT("Bool value is: %s"), TCPSocket->HasPendingData(Size) ? TEXT("true") : TEXT("false"));
	//Binary Array!
	TArray<uint8> ReceivedData;


	while (TCPSocket->HasPendingData(Size))
	{

		//ReceivedData.Init(FMath::Min(Size, 65507u));
		ReceivedData.SetNumZeroed(Size);
		int32 Read = 0;
		TCPSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
}

//Rama's String From Binary Array
FString ASocketServer::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	return FString(cstr.c_str());
}

// Called when the game starts or when spawned
void ASocketServer::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Loaded"));
	ASocketServer::Connect("127.0.0.1", 3000);
	ASocketServer::TCPSocketListener();
}

// Called every frame
void ASocketServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ASocketServer::TCPSocketListener();
}

