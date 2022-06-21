// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGeneratorBase.h"
#include "RogueBeta_MapGenerator.generated.h"

/**
 区域分割のアルゴリズム(安藤作)の実装
 ＜利用方法＞
 1. コンストラクターに縦横のサイズを渡し、オブジェクトを作成する。
 2. 必要であれば、セッターを使ってパラメーターを設定する。
 3. Generate_Map()を実行すると、マップが生成された状態になる。
 4. マップの生成後は、GetCell()によって各マスが何であるかを取得できる。

 ＜パラメーター一覧＞
 int32 MaxTrial: 区域分割の、最大試行回数
 int32 MinDivisionTarget: 分割対象の最小長辺長
 int32 MinSize: 区域の最小辺長

 ＜生成手順＞
 1. 区域を長方形に分割していく
 最大試行回数に到達するか、分割対象の最小長辺長を満たすものがなくなるまで、
 対象の条件を満たす中で面積が最大の長方形を、短辺に平行に、最小辺長より短い辺が現れない範囲でランダムに分割する。

 2. 部屋を生成する
 各区域で、外周の一マスを除いた部分を部屋とする。

 3. 通路を生成する

 */
UCLASS()
class TAG_ROGUE_API URogueBeta_MapGenerator : public UMapGeneratorBase
{
	GENERATED_BODY()
protected:
	TMap<EType, int32> StructureSize;

	//パラメーター
	int32 MaxTrial = 35;//区域分割の、最大試行回数
	int32 MinDivisionTarget = 15;//分割対象の最小長辺長
	int32 MinSize = 3;//区域の最小辺長

public:
	URogueBeta_MapGenerator() {}

	//マップの縦横サイズを引数にとってマップを生成するコンストラクタ
	URogueBeta_MapGenerator(int32, int32);

	//パラメーターのセッター
	void SetMaxTrial(int32 value) {
		if (value >= 0) {
			MaxTrial = value;
		}
	}
	void SetMinDivisionTarget(int32 value) {
		if (value >= 0) {
			MinDivisionTarget = value;
		}
	}
	void SetMinSize(int32 value) {
		if (value >= 3) {
			MinSize = value;
		}
	}
};
