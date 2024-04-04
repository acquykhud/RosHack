#pragma once
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

float distance(D3DVECTOR x, D3DVECTOR y);
float decryptFloat(DWORD encryptedFloat, DWORD key);
bool W2S(D3DVECTOR pos, D3DVECTOR &screen, D3DXMATRIX matrix, int windowWidth, int windowHeight);
const char* getItem(int Item);
int getItemLevel(int Item);
//DrawString(cFont, Screen.x, Screen.y, RED, (PCHAR)getItem(Entity->m_Table->pTable->Prop->mVal));