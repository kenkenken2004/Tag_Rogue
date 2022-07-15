// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapComponent.h"

#include "CADKernel/UI/Display.h"
#include "Net/UnrealNetwork.h"
#include "Tag_Rogue/Character/CharacterBase.h"


// Sets default values for this component's properties
UMiniMapComponent::UMiniMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UMiniMapComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UMiniMapComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UMiniMapComponent::InitializeByServer()
{
	if(!IsValid(GameInstance))
	{
		OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
		GameInstance = UTag_RogueGameInstance::GetInstance();
		GameInstance->InitializeMapBuilders();
		Generater = GameInstance->MapGenerator;
		Maker = GameInstance->TerrainMaker;
		GameInstance->LoadAssets();
		DisplayMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Display"));
		SetStaticMesh(DisplayMesh);
	}
	
	MapHeight = Generater->MapHeight;
	MapWidth = Generater->MapWidth;
	CellSize = Maker->CellSize;
	Scale = GameInstance->MapScale;
	TextureBitArray = TArray<bool>();
	for (int32 y=0;y<MapHeight;y++)
	{
		for(int32 x=0;x<MapWidth;x++)
		{
			TextureBitArray.Add(Generater->GetCell(y,x)->Attribution==URogueAlpha_MapGenerator::EType::Wall);
		}
	}
}

void UMiniMapComponent::Initialize_Implementation()
{
	if(!IsValid(GameInstance))
	{
		OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
		GameInstance = UTag_RogueGameInstance::GetInstance();
		GameInstance->InitializeMapBuilders();
		Generater = GameInstance->MapGenerator;
        Maker = GameInstance->TerrainMaker;
        GameInstance->LoadAssets();
		DisplayMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Display"));
		SetStaticMesh(DisplayMesh);
	}
	
	MapMaterial = CreateAndSetMaterialInstanceDynamic(0);
	MapMaterial->SetTextureParameterValue(TEXT("MiniMap"),CreateMiniMapTexture());
	MapMaterial->SetScalarParameterValue(TEXT("MapHeight"),MapHeight*CellSize);
	MapMaterial->SetScalarParameterValue(TEXT("MapWidth"),MapWidth*CellSize);
	MapMaterial->SetScalarParameterValue(TEXT("Scale"),Scale);
}

void UMiniMapComponent::UpdateMapDirection_Implementation()
{
	if(!IsValid(GameInstance))Initialize();
	MapMaterial->SetScalarParameterValue(TEXT("Rotation"),(OwnerPlayer->GetControlRotation().Yaw+90)/360.0);
}

UTexture* UMiniMapComponent::CreateMiniMapTexture() const
{
	// Texture Information
	const int Width = MapWidth;
	const int Height = MapHeight;
	uint8 * Pixels = static_cast<uint8*>(malloc(Height * Width * 4)); // x4 because it's RGBA. 4 integers, one for Red, one for Green, one for Blue, one for Alpha
	// filling the pixels with dummy data (4 boxes: red, green, blue and white)
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			const int32 Val = TextureBitArray[y*Width+x] ?255:0;
			Pixels[y * 4 * Width + x * 4 + 0] = Val; // R
			Pixels[y * 4 * Width + x * 4 + 1] = Val; // G
			Pixels[y * 4 * Width + x * 4 + 2] = Val; // B
			Pixels[y * 4 * Width + x * 4 + 3] = Val; // A
		}
	}
	// Create Package
	// Create the Texture
	UTexture2D* Texture = NewObject<UTexture2D>();
	// Texture Settings
	Texture->SetPlatformData(new FTexturePlatformData());
	Texture->GetPlatformData()->SizeX = Width;
	Texture->GetPlatformData()->SizeY = Height;
	Texture->GetPlatformData()->PixelFormat = PF_R8G8B8A8;

	// Passing the pixels information to the texture
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	Texture->GetPlatformData()->Mips.Add(Mip);
	Mip->SizeX = Width;
	Mip->SizeY = Height;
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = static_cast<uint8*>(Mip->BulkData.Realloc(Height * Width * sizeof(uint8) * 4));
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * Height * Width * 4);
	Mip->BulkData.Unlock();
	// Updating Texture & mark it as unsaved
	Texture->AddToRoot();
	Texture->UpdateResource();
	free(Pixels);
	Pixels = nullptr;
	return Texture;
}

void UMiniMapComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMiniMapComponent, OwnerPlayer);
	DOREPLIFETIME(UMiniMapComponent, DisplayMesh);
	DOREPLIFETIME(UMiniMapComponent, CellSize);
	DOREPLIFETIME(UMiniMapComponent, MapHeight);
	DOREPLIFETIME(UMiniMapComponent, MapWidth);
	DOREPLIFETIME(UMiniMapComponent, Scale);
	DOREPLIFETIME(UMiniMapComponent, TextureBitArray);
}
