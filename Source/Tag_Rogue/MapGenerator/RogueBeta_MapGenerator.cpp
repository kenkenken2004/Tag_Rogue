// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBeta_MapGenerator.h"

void URogueBeta_MapGenerator::Construct(int32 Map_Height, int32 Map_Width) {
	Super::Construct(Map_Height, Map_Width);
}

void URogueBeta_MapGenerator::GenerateMap(int32 MaxTrial, int32 MinDivisionTarget, int32 MinSize) {
	//
	//1. 初期化する
	//


	//区域全体をAreaListに追加
	FArea* WholeArea = new FArea(&MapMatrix[0][0], &MapMatrix[MapHeight - 1][MapWidth - 1]);
	WholeArea->Place();

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



	//
	//3. 部屋を生成する
	//
	for (int i = 0; i < AreaList.Num(); i++) {
		FCell Cell1 = MapMatrix[AreaList[i]->LeftTopCell->Py + 1][AreaList[i]->LeftTopCell->Px + 1];//作る部屋の左上のCell
		FCell Cell2 = MapMatrix[AreaList[i]->RightBottomCell->Py - 1][AreaList[i]->RightBottomCell->Px - 1];//作る部屋の右下のCell
		FSpace* NewSpace = new FSpace(&Cell1, &Cell2, EType::Room);
		NewSpace->Place();
	}


	//
	//4. 通路を生成する
	//

	//FSpaceとして各々の道の領域を指定し、属性を設定して配置する

	//横向きの境界を見る
	for (int i = 0; i < MapWidth - 1; i++) {
		int tmpLeft = -1;//ひとつ前に見た領域の左右の領域番号
		int tmpRight = -1;
		int startHeight = -1;//同じ領域の組の境界が連続している中で最初のheight(j)
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

	//縦向きの境界を見る
	for (int i = 0; i < MapHeight - 1; i++) {
		int tmpTop = -1;//ひとつ前に見た領域の左右の領域番号
		int tmpBottom = -1;
		int startWidth = -1;//同じ領域の組の境界が連続している中で最初のheight(j)
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