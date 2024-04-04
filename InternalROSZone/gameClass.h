#pragma once
#pragma once
#include <Windows.h>
#include "directX.h"
#include <vector>

//ClientApp: 3B 86 94 0E 00 00 74 0F
//ThisFunc return MainApp : E8 ? ? ? ? 8B 78 10
//GetEntityById : E8 ? ? ? ? 8B F0 89 75 D8

// \x89\x46\x00\x5e\x5d\xc2\x00\x00\xcc\xcc\xcc\x55\x8b\xec\x8b\x45\x00\x89\x81 xx?xxx??xxxxxxxx?xx : worldMatrix (viewMatrix) (esi)
// \x8b\x49\x00\x89\x45 xx?xx : worldMatrix (viewMatrix) (ecx)


typedef DWORD(__thiscall *fDecrypt)(void* ClientApp, DWORD id, int a3); //a3 = always 0

struct Offset
{
	static const DWORD clientApp_ptr = 0x20B9128;// 0x20B7020;//   m_ClientApp = *(ClientApp**)(base + clientApp_ptr);
	static const DWORD fDec = 0x167770; // 0x1671A0; // oriDec = (fDecrypt)(base + 0x170600);
	static const DWORD sceneContext = 0x1EAFDAC; // 0x1EADCDC;
	static const DWORD inGame = 0x1DF2000; // 0x1DF0000;
};

class Meta
{
public:
	char _0x0000[4];
	void* m_pMeta; //0x0004 
	char _0x0008[4];
	char* pName; //0x000C  Land, Athlete, Plane, PlayerAvatar
};

class mHP
{
public:
	char pad_0x0000[0x8]; //0x0000
	__int32 HP; //0x0008 

}; //Size=0x000C

class mNick
{
public:
	char pad_0x0000[0x8]; //0x0000
	__int32 nickLen; //0x0008 
	char pad_0x000C[0x8]; //0x000C
	char m_NickName[4]; // 0x0014

}; //Size=0x0018

class mSimple
{
public:
	char pad_0x0000[0x8]; //0x0000
	__int32 mVal; //0x0008 

}; //Size=0x000C


class mTablePtr
{
public:
	char pad_0000[0x2C]; //0x0000
	class mSimple* Prop; //0x002C
	char pad_0030[0x8]; //0x0030
	class mHer* m_HerRow; //0x003C 
	char pad_003C[0x14]; //0x003C
	class mHP* m_Hpbot; //0x0050
	char pad_0054[0x50]; //0x0054
	class mHP* m_HpRow; //0x00A4
	char pad_A8[0x8]; //0x00A8
	class mHP* m_MaxHpRow; //0x00B0
	char pad_00B4[0xA4]; //0x00B4
	class mSimple* m_TeamRow; //0x0158
	char pad_00144[0x20]; //0x0015C
	class mSimple* m_PlayerKill; //0x017C
	char pad_0168[0x38]; //0x0180
	char pad_xxx[0x24]; // 0x 
	class mSimple* m_PoseRow; //0x01DC
	char pad_01A4[0x158]; //0x1BC
	//class mNick* m_NickRow; //0x314
};

class mTable
{
public:
	char pad_0x0000[0x14]; //0x0000
	mTablePtr* pTable; //0x0014 
}; //Size=0x0018

class mKey {
public:
	DWORD Val;
};

class CEntity {
public:
	char pad_0000[4]; //0x0000
	class Meta* m_pMeta; //0x0004
	char pad_0008[8]; //0x0008
	DWORD EncryptedX; //0x0010
	class mKey* XKey; //0x0014
	DWORD EncryptedY; //0x0018
	class mKey* YKey; //0x001C
	DWORD EncryptedZ; //0x0020
	class mKey* ZKey; //0x0024
	char pad_0028[272]; //0x0028
	class mTable* m_Table; // 0x00138
};

class ClientApp
{
public:
	class ListItem
	{
	public:
		ListItem * m_pNext; //0x0000 
		ListItem* m_pPrevious; //0x0004 
		DWORD m_ObjectId; //0x0008 
		DWORD m_pEntity; //0x000C
		mKey*	m_EntityKey; //0x0010
	};
	class LinkedList
	{
	public:
		ListItem* m_pStart;
		ListItem* m_pEnd;
	};
	char pad_0x0000[0xE94];
	LinkedList* m_pFirst; //0x0E94
	__int32 m_UnkObjectCount; //0x0E98 
	LinkedList** m_ppObjects; //0x0E9C 
};


class BoneNameEntry
{
public:
	char m_name[100][32];
};//Size=0x0020

class BoneMatrixEntry
{
public:
	D3DXMATRIX m_matrices[100];
};

class Animator
{
public:
	char pad_0x0000[32];
	WORD m_boneCount; //0x0020 
	char pad_0x0022[1282];
	BoneNameEntry* m_pBoneNames; //0x0524
	char* m_modelName; //0x0528 
	char pad_0x052C[1092];
	BoneMatrixEntry* m_pBoneMatrices; //0x0970 
	char pad_0x0974[12];
	const char* GetBoneNameByIndex(unsigned int uiBoneIndex)
	{
		if (m_pBoneNames && uiBoneIndex < m_boneCount)
			return (const char*)&(m_pBoneNames->m_name[uiBoneIndex]);
		return (const char*)&"";
	}

	bool GetBoneIndexByName(const char* pBoneName, unsigned int* pIndexOut)
	{

		if (pBoneName == NULL || pIndexOut == NULL)
			return false;

		for (unsigned int i = 0; i < m_boneCount; i++)
		{
			const char* pName = GetBoneNameByIndex(i);
			if (pName && strcmp(pBoneName, pName) == 0)
			{
				*pIndexOut = i;
				return true;
			}
		}
		return false;
	}

	D3DXMATRIX* getMatrixByBoneIndex(unsigned int uiBoneIndex)
	{
		if (m_pBoneMatrices && uiBoneIndex < m_boneCount)
			return (D3DXMATRIX*)&(m_pBoneMatrices->m_matrices[uiBoneIndex]);
		return NULL;
	}
};//Size=0x0980

class Model
{
public:
	char pad_0x0000[28];
	void* m_pNode; //0x001C 
	char pad_0x0020[776];
	Animator* m_pAnimator; //0x0328 
	char pad_0x032C[132];
	D3DXMATRIX m_transform; //0x03B0 
	char pad_0x03F0[80];
};//Size=0x0440

class Camera
{
public:
	char pad_0x0000[4];
	D3DXMATRIX m_matrix; //0x0004 
	D3DXMATRIX m_matrixInverse; //0x0044 
	char pad_0x0084[64];
	D3DXMATRIX m_worldMatrix; //0x00C4: viewMatrix
};//Size=0x0104

class SceneContext
{
public:
	char pad_0x0000[4];
	Camera* m_pCamera; //0x0004 
};//Size=0x0040

class BigWorld
{
public:
	virtual void Function0(); //
	char pad_0x0004[4];
	SceneContext* m_pScene; //0x0008 
	char pad_0x000C[488];
	D3DXMATRIX m_unknown; //0x01F4 
	char pad_0x0234[72];
	Model* m_pLocalModel; //0x027C 
	char pad_0x0280[16];
	std::vector<Model*> m_pVisModels; //0x0290 
	char pad_0x029C[368];
};//Size=0x040C