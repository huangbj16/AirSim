// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleGenerator.h"
#include "Engine/StaticMeshActor.h"
#include <fstream>
#include <string>
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

// Sets default values
AObstacleGenerator::AObstacleGenerator()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Read transform data from file
    FString FilePath = FPaths::ProjectContentDir() + "Settings/cubes.txt";
    std::ifstream File(TCHAR_TO_ANSI(*FilePath)); // Convert FString to std::string
    std::string Line;
    while (std::getline(File, Line)) {
        TSharedPtr<FJsonObject> JsonObject;
        FString Fline = FString(Line.c_str());
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Fline);

        if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
            FVector Location(JsonObject->GetNumberField("LocationX"), JsonObject->GetNumberField("LocationY"), JsonObject->GetNumberField("LocationZ"));
            FRotator Rotation(JsonObject->GetNumberField("RotationPitch"), JsonObject->GetNumberField("RotationYaw"), JsonObject->GetNumberField("RotationRoll"));
            FVector Scale(JsonObject->GetNumberField("ScaleX"), JsonObject->GetNumberField("ScaleY"), JsonObject->GetNumberField("ScaleZ"));
            FTransform NewTransform(Rotation, Location, Scale);
            ActorTransforms.Add(NewTransform);
        }
    }
}

// Called when the game starts or when spawned
void AObstacleGenerator::BeginPlay()
{
    Super::BeginPlay();

    int count = 0;
    for (const FTransform& Transform : ActorTransforms) {
        // Spawn parameters
        count++;
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        // SpawnParams.Name = FName("obs");

        // Spawn the cube actor
        AStaticMeshActor* SpawnedCube = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform, SpawnParams);

        if (SpawnedCube) {
            // Use Unreal's default cube mesh
            UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));

            if (CubeMesh) {
                // Set the cube mesh
                SpawnedCube->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
            }
        }
    }
    printf("count = %d\n", count);
}

// Called every frame
void AObstacleGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

