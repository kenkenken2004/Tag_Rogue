// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"

#include "Tag_Rogue/Map/TerrainMaker.h"


// Sets default values
AMiniMap::AMiniMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Display = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display"));
	SetRootComponent(Display);
}

void AMiniMap::Initialize(URogueAlpha_MapGenerator* Gen, UTerrainMaker* Ter)
{
	Generater = Gen;
	Maker = Ter;
	GameInstance = static_cast<UTag_RogueGameInstance*>(GetGameInstance());
	GameInstance->LoadAssets();
	DisplayMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Display"));
	Display->SetStaticMesh(DisplayMesh);
	MapMaterial = Display->CreateAndSetMaterialInstanceDynamic(0);
	MapMaterial->SetTextureParameterValue(TEXT("MiniMap"),CreateMiniMapTexture());
	MapMaterial->SetScalarParameterValue(TEXT("MapHeight"),Generater->MapHeight*Maker->CellSize);
	MapMaterial->SetScalarParameterValue(TEXT("MapWidth"),Generater->MapWidth*Maker->CellSize);
	MapMaterial->SetScalarParameterValue(TEXT("Scale"),0.125);
}

// Called when the game starts or when spawned
void AMiniMap::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMiniMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MapMaterial->SetScalarParameterValue(TEXT("Rotation"),GetOwner()->GetActorRotation().Yaw/360.0);
}

UTexture* AMiniMap::CreateMiniMapTexture() const
{
	// Texture Information
	const int Width = Generater->MapWidth;
	const int Height = Generater->MapHeight;
	uint8 * Pixels = static_cast<uint8*>(malloc(Height * Width * 4)); // x4 because it's RGBA. 4 integers, one for Red, one for Green, one for Blue, one for Alpha
	// filling the pixels with dummy data (4 boxes: red, green, blue and white)
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			const int32 Val = (Generater->GetCell(y,x)->Attribution==URogueAlpha_MapGenerator::EType::Wall)?255:0;
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
	return static_cast<UTexture2D*>(Texture);
}
