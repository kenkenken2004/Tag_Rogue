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
 1. 初期化する
 区域全体を、領域リストに追加する。

 2. 区域を長方形に分割していく
 最大試行回数に到達するか、分割対象の最小長辺長を満たすものがなくなるまで、
 対象の条件を満たす中で面積が最大の長方形を、短辺に平行に、最小辺長より短い辺が現れない範囲でランダムに分割する。

 3. 部屋を生成する
 各区域で、外周の一マスを除いた部分を部屋とする。

 4. 通路を生成する

 */
UCLASS()
class TAG_ROGUE_API URogueBeta_MapGenerator : public UMapGeneratorBase
{

	public:
		GENERATED_BODY()
		URogueBeta_MapGenerator() {}

		void Construct(int32 Map_Height, int32 Map_Width) override;

		//パラメーターを引数にとってマップを生成するコンストラクタ
		void GenerateMap(int32 MaxTrial, int32 MinDivisionTarget, int32 MinSize);

		void PrintMap();
};
