// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBeta_MapGenerator.h"

URogueBeta_MapGenerator::URogueBeta_MapGenerator(int32 MapHeight, int32 MapWidth) :UMapGeneratorBase(MapHeight,MapWidth) {
	//
	//1. ����������
	//

	//���S�̂�AreaList�ɒǉ�
	FArea* WholeArea = new FArea(CellList[MapMatrix[0][0]], CellList[MapMatrix[MapHeight - 1][MapWidth - 1]]);
	AreaList.Add(WholeArea);
	
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

	//���������̈��z�u����
	for (int i = 0; i < AreaList.Num(); i++) {
		AreaList[i]->Place();
	}

	//
	//3. �����𐶐�����
	//
	for (int i = 0; i < AreaList.Num(); i++) {
		FCell Cell1 = CellList[MapMatrix[AreaList[i]->LeftTopCell->Px + 1][AreaList[i]->LeftTopCell->Py + 1]];//��镔���̍����Cell
		FCell Cell2 = CellList[MapMatrix[AreaList[i]->RightBottomCell->Px - 1][AreaList[i]->RightBottomCell->Py - 1]];//��镔���̉E����Cell
		FSpace* NewSpace = new FSpace(Cell1, Cell2, EType::Room);
		SpaceList.Add(NewSpace);
		SpaceList.Last()->Place();
	}

	//
	//4. �ʘH�𐶐�����
	//

}