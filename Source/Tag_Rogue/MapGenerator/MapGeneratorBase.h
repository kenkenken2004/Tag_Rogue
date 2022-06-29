// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGeneratorBase.generated.h"

/**
 ・FCellをマップの最小構成要素とする。
 ・FRectはFCellの長方形上の集合である。
 ・FSpaceはFRectの派生クラスであり、実際にマップに配置する部屋類である。
 ・FPathも同様であり、FSpaceや自身間をつなぐ通路類で、形状はいずれかの辺が1Cellの長方形である。
 ・FAreaはFRectの派生クラスであり、抽象的領域を表し、マップには配置されない。拡張や分割といった操作を行える。
 ・基本的にこのUMapGeneratorBaseクラスを基底クラスとして派生させ、各マップアルゴリズムのクラスを実装する。
 */
UCLASS()
class TAG_ROGUE_API UMapGeneratorBase: public UObject
{
protected:
	GENERATED_BODY()

	int32 MapHeight = 1;
	int32 MapWidth = 1; //マップのサイズ
	
	enum struct EType {//マス・部屋の属性
		Wall,
		Plaza,
		Room,
		Path
	}; 
	
	enum struct EDirection {//方向
		North,
		East,
		West,
		South
	};
	
	struct FCell //マスの構造体
	{
		int32 Py, Px; //二次元配列における座標。アクセスはArray[Py][Px]。
		int32 AreaIndex = -1; //所属する領域のリストインデックス。所属していない場合は-1。
		EType Attribution; //Cellの属性。
		UMapGeneratorBase* Gen; //所有されるMapGenerator
		FCell(int32, int32, UMapGeneratorBase*); //属性無しのコンストラクタ。このとき属性はWallとなる。
		FCell(int32, int32, EType, UMapGeneratorBase*); //属性ありのコンストラクタ。
	};
	struct FRect //マスの長方形の領域の構造体
	{
		int32 Index = -1; //SpaceListまたはAreaListのインデックス。所属しない場合は-1。
		FCell *LeftTopCell; //左上の角に位置するFCellのポインタ。
		FCell *RightBottomCell; //右下の角に位置するFCellのポインタ。
		int32 Height; //Y方向の辺の長さ。
		int32 Width; //Z方向の辺の長さ。
		UMapGeneratorBase* Gen; //同上
		FRect(FCell*,FCell*); //角を指定するコンストラクタ。
		FRect(FCell*, int32, int32);//左上端と、辺の長さを指定するコンストラクタ。
		TArray<FCell*> GetInnerBorderCells(EDirection) const; //周上の内側のCellを取得
		TArray<FCell*> GetOuterBorderCells(EDirection) const; //周上の外側のCellを取得
		TArray<FCell*> GetAllCells() const; //構成するCellを取得
		FCell* GetCenterCell() const;
	};
	struct FSpace: FRect //部屋などの構造体
	{
		EType Attribution; //部屋の種類
		FSpace(FCell*, FCell*, EType); //FCellと属性を入力するコンストラクタ。
		void ChangeAttr(EType); //部屋の種類を変更
		bool CanPlace() const; //構造体としてマップに配置可能か判断。
		void Place(); //構造体としてマップに配置(可能かは判断しない)。
		void Remove(); //マップから削除。
	};
	struct FPath //通路などの構造体
	{
		int32 Index;
		UMapGeneratorBase* Gen;
		FSpace *Node1;
		FSpace *Node2;
		FCell* End1;
		FCell* End2;
		TArray<FCell*> Cells;//代入時に、End1からEnd2までの道筋順になることを要請する。
		int32 Length; //通路の長さ。
		FPath(FSpace*, FSpace*, TArray<FCell*>, FCell*, FCell*); //つなぐ対象と本体のFRectを指定するコンストラクタ。
		bool CanPlace() const; //同上
		void Place(); //同上 ただ、通路の両端が場合によっては変化することに注意。
		void Remove(); //同上
	};
	struct FArea: FRect //仮想的な領域の構造体
	{
		FSpace *Owner;
		FArea(FCell*, FCell*, FSpace*); //角と所有対象の構造体を入力するコンストラクタ。
		FArea(FCell*, FCell*);
		TArray<FArea*> Split(EDirection, int32); //EDirection側をint32だけ分割する。自身はRemoveされ新たに二つのFAreaが配置される。
		void Expand(EDirection, int32); //指定方向に指定Cell数だけ拡張。可能かどうかは判定しない。
		bool Expand(); //全方向に1Cellずつ拡張。できない方向へは拡張しない。
		bool CanPlace() const; //同上
		void Place(); //同上(仮想領域として)
		void Remove(); //同上
	};
	
	TArray<FSpace*> SpaceList; //部屋類のポインタのリスト。
	TArray<FArea*> AreaList; //仮想領域のポインタのリスト。
	TArray<FPath*> PathList; //通路類のポインタのリスト。
	
	TArray<TArray<FCell>> MapMatrix; //CellListへの参照indexが入っている、マップを表現した二次元リスト。
	
public:
	UMapGeneratorBase();
	UMapGeneratorBase(int32, int32); //マップのサイズを指定するコンストラクタ。(sizeY, sizeX), Array[Y][X]
	FCell* GetCell(int32, int32); //指定座標のFCellを取得。
	virtual void Construct(int32, int32);
};
