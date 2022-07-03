// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBeta_MapGenerator.h"

void URogueBeta_MapGenerator::Construct(int32 Map_Height, int32 Map_Width) {
	Super::Construct(Map_Height, Map_Width);
}

void URogueBeta_MapGenerator::GenerateMap(int32 MaxTrial, int32 MinDivisionTarget, int32 MinSize) {
	//
	//1. ����������
	//


	//���S�̂�AreaList�ɒǉ�
	FArea* WholeArea = new FArea(&MapMatrix[0][0], &MapMatrix[MapHeight - 1][MapWidth - 1]);
	WholeArea->Place();

	//
	//2. ���𒷕��`�ɕ������Ă���
	//

	for (int i = 0; i < MaxTrial; i++) {
		//���ꂩ�番������̈��I��
		//���ӂ̒�����MinDivisionTarget��MinSize*2�ȏ�ł��钆�ŁA�ʐς��ő�̂��̂�I��
		int ToDivideId = -1;
		int ToDivideAreaSize = 0;
		for (int j = 0; j < AreaList.Num(); j++) {
			int AreaHeight = AreaList[j]->Height;
			int AreaWidth = AreaList[j]->Width;
			if (AreaHeight * AreaWidth > ToDivideAreaSize && FMath::Max(AreaHeight, AreaWidth) >= FMath::Max(MinDivisionTarget, MinSize * 2)) {
				ToDivideId = j;
				ToDivideAreaSize = AreaHeight * AreaWidth;
			}
		}

		//�����Ώۂ��Ȃ��Ȃ�����I���
		if (ToDivideId == -1)break;

		//��������
		if (AreaList[ToDivideId]->Height >= AreaList[ToDivideId]->Width) {//�c�����ȏ�̏ꍇ���ɕ���
			//�㑤�̂�����؂藣���������_���Ɍ��߂�
			int border = FMath::RandRange(MinSize, AreaList[ToDivideId]->Height - MinSize);
			//�؂藣��
			AreaList[ToDivideId]->Split(EDirection::South, border);
		}
		else {//���̕��������ꍇ���ɕ���
			int border = FMath::RandRange(MinSize, AreaList[ToDivideId]->Width - MinSize);
			//�؂藣��
			AreaList[ToDivideId]->Split(EDirection::West, border);

		}

	}



	//
	//3. �����𐶐�����
	//
	for (int i = 0; i < AreaList.Num(); i++) {
		FCell Cell1 = MapMatrix[AreaList[i]->LeftTopCell->Py + 1][AreaList[i]->LeftTopCell->Px + 1];//��镔���̍����Cell
		FCell Cell2 = MapMatrix[AreaList[i]->RightBottomCell->Py - 1][AreaList[i]->RightBottomCell->Px - 1];//��镔���̉E����Cell
		FSpace* NewSpace = new FSpace(&Cell1, &Cell2, EType::Room);
		NewSpace->Place();
	}


	//
	//4. �ʘH�𐶐�����
	//

	//FSpace�Ƃ��Ċe�X�̓��̗̈���w�肵�A������ݒ肵�Ĕz�u����

	//�������̋��E������
	for (int i = 0; i < MapWidth - 1; i++) {
		int tmpLeft = -1;//�ЂƂO�Ɍ����̈�̍��E�̗̈�ԍ�
		int tmpRight = -1;
		int startHeight = -1;//�����̈�̑g�̋��E���A�����Ă��钆�ōŏ���height(j)
		for (int j = 0; j <= MapHeight; j++) {
			if (j != 0 && (j == MapHeight || MapMatrix[j][i].AreaIndex != tmpLeft || MapMatrix[j][i + 1].AreaIndex != tmpRight)) {
				if (j - startHeight >= 3 && tmpLeft != tmpRight) {
					int pathHeight = FMath::RandRange(startHeight + 1, j - 2);
					FSpace newPath = FSpace(&MapMatrix[pathHeight][i], &MapMatrix[pathHeight][i + 1], EType::Path);
					newPath.Place();
				}
			}

			if (j != MapHeight && (j == 0 || MapMatrix[j][i].AreaIndex != tmpLeft || MapMatrix[j][i + 1].AreaIndex != tmpRight)) {
				tmpLeft = MapMatrix[j][i].AreaIndex;
				tmpRight = MapMatrix[j][i + 1].AreaIndex;
				startHeight = j;
			}
		}
	}

	//�c�����̋��E������
	for (int i = 0; i < MapHeight - 1; i++) {
		int tmpTop = -1;//�ЂƂO�Ɍ����̈�̍��E�̗̈�ԍ�
		int tmpBottom = -1;
		int startWidth = -1;//�����̈�̑g�̋��E���A�����Ă��钆�ōŏ���height(j)
		for (int j = 0; j <= MapWidth; j++) {
			if (j != 0 && (j == MapWidth || MapMatrix[i][j].AreaIndex != tmpTop || MapMatrix[i + 1][j].AreaIndex != tmpBottom)) {
				if (j - startWidth >= 3 && tmpTop != tmpBottom) {
					int pathWidth = FMath::RandRange(startWidth + 1, j - 2);
					FSpace newPath = FSpace(&MapMatrix[i][pathWidth], &MapMatrix[i + 1][pathWidth], EType::Path);
					newPath.Place();
				}
			}

			if (j != MapWidth && (j == 0 || MapMatrix[i][j].AreaIndex != tmpTop || MapMatrix[i + 1][j].AreaIndex != tmpBottom)) {
				tmpTop = MapMatrix[i][j].AreaIndex;
				tmpBottom = MapMatrix[i + 1][j].AreaIndex;
				startWidth = j;
			}
		}
	}

	PrintMap();
}

void URogueBeta_MapGenerator::PrintMap() {
	FString output;
	char cellChar;
	for (int i = 0; i < MapHeight; i++) {
		output = "";
		for (int j = 0; j < MapWidth; j++) {
			if (MapMatrix[i][j].Attribution == EType::Room || MapMatrix[i][j].Attribution == EType::Path) {
				cellChar = ' ';
			}
			else {
				cellChar = static_cast <char>('A' + MapMatrix[i][j].AreaIndex);
			}
			output += cellChar;
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *output);
	}
}