// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGeneratorBase.h"
#include "RogueBeta_MapGenerator.generated.h"

/**
 ��敪���̃A���S���Y��(������)�̎���
 �����p���@��
 1. �R���X�g���N�^�[�ɏc���̃T�C�Y��n���A�I�u�W�F�N�g���쐬����B
 2. �K�v�ł���΁A�Z�b�^�[���g���ăp�����[�^�[��ݒ肷��B
 3. Generate_Map()�����s����ƁA�}�b�v���������ꂽ��ԂɂȂ�B
 4. �}�b�v�̐�����́AGetCell()�ɂ���Ċe�}�X�����ł��邩���擾�ł���B

 ���p�����[�^�[�ꗗ��
 int32 MaxTrial: ��敪���́A�ő厎�s��
 int32 MinDivisionTarget: �����Ώۂ̍ŏ����Ӓ�
 int32 MinSize: ���̍ŏ��Ӓ�

 �������菇��
 1. ���𒷕��`�ɕ������Ă���
 �ő厎�s�񐔂ɓ��B���邩�A�����Ώۂ̍ŏ����Ӓ��𖞂������̂��Ȃ��Ȃ�܂ŁA
 �Ώۂ̏����𖞂������Ŗʐς��ő�̒����`���A�Z�ӂɕ��s�ɁA�ŏ��Ӓ����Z���ӂ�����Ȃ��͈͂Ń����_���ɕ�������B

 2. �����𐶐�����
 �e���ŁA�O���̈�}�X�������������𕔉��Ƃ���B

 3. �ʘH�𐶐�����

 */
UCLASS()
class TAG_ROGUE_API URogueBeta_MapGenerator : public UMapGeneratorBase
{
	GENERATED_BODY()
protected:
	TMap<EType, int32> StructureSize;

	//�p�����[�^�[
	int32 MaxTrial = 35;//��敪���́A�ő厎�s��
	int32 MinDivisionTarget = 15;//�����Ώۂ̍ŏ����Ӓ�
	int32 MinSize = 3;//���̍ŏ��Ӓ�

public:
	URogueBeta_MapGenerator() {}

	//�}�b�v�̏c���T�C�Y�������ɂƂ��ă}�b�v�𐶐�����R���X�g���N�^
	URogueBeta_MapGenerator(int32, int32);

	//�p�����[�^�[�̃Z�b�^�[
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
