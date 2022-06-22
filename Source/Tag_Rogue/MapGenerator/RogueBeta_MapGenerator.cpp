// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBeta_MapGenerator.h"

URogueBeta_MapGenerator::URogueBeta_MapGenerator(int32 MapHeight, int32 MapWidth) :UMapGeneratorBase(MapHeight,MapWidth) {
	//
	//1. 初期化する
	//

	//区域全体をAreaListに追加
	FArea* WholeArea = new FArea(CellList[MapMatrix[0][0]], CellList[MapMatrix[MapHeight - 1][MapWidth - 1]]);
	AreaList.Add(WholeArea);
	
	//
	//2. 区域を長方形に分割していく
	//

	for (int i = 0; i < MaxTrial; i++) {
		//これから分割する領域を選ぶ
		//長辺の長さがMinDivisionTargetとMinSize*2以上である中で、面積が最大のものを選ぶ
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

		//分割対象がなくなったら終わり
		if (ToDivideId == -1)break;

		//分割する
		if (AreaList[ToDivideId]->Height >= AreaList[ToDivideId]->Width) {//縦が横以上の場合横に分割
			//上側のいくつを切り離すかランダムに決める
			int border = FMath::RandRange(MinSize, AreaList[ToDivideId]->Height - MinSize);
			//切り離す
			AreaList[ToDivideId]->Split(EDirection::South, border);
		}
		else {//横の方が長い場合横に分割
			int border = FMath::RandRange(MinSize, AreaList[ToDivideId]->Width - MinSize);
			//切り離す
			AreaList[ToDivideId]->Split(EDirection::West, border);

		}

	}

	//分割した領域を配置する
	for (int i = 0; i < AreaList.Num(); i++) {
		AreaList[i]->Place();
	}

	//
	//3. 部屋を生成する
	//
	for (int i = 0; i < AreaList.Num(); i++) {
		FCell Cell1 = CellList[MapMatrix[AreaList[i]->LeftTopCell->Px + 1][AreaList[i]->LeftTopCell->Py + 1]];//作る部屋の左上のCell
		FCell Cell2 = CellList[MapMatrix[AreaList[i]->RightBottomCell->Px - 1][AreaList[i]->RightBottomCell->Py - 1]];//作る部屋の右下のCell
		FSpace* NewSpace = new FSpace(Cell1, Cell2, EType::Room);
		SpaceList.Add(NewSpace);
		SpaceList.Last()->Place();
	}

	//
	//4. 通路を生成する
	//

}