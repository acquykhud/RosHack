#include "myClass.h"

float distance(D3DVECTOR x, D3DVECTOR y)
{
	float res = 0.0f;
	res += (x.x - y.x) * (x.x - y.x);
	res += (x.y - y.y) * (x.y - y.y);
	res += (x.z - y.z) * (x.z - y.z);
	return sqrtf(res);
}

float decryptFloat(DWORD encryptedFloat, DWORD key)
{
	DWORD tmp = encryptedFloat ^ key;
	return *reinterpret_cast<float*>(&tmp);
};


bool W2S(D3DVECTOR pos, D3DVECTOR &screen, D3DXMATRIX matrix, int windowWidth, int windowHeight)
{
	float x = 0.f, y = 0.f, z = 0.f, w = 0.f;

	x = pos.x*matrix._11 + pos.y*matrix._21 + pos.z*matrix._31 + matrix._41;
	y = pos.x*matrix._12 + pos.y*matrix._22 + pos.z*matrix._32 + matrix._42;
	z = pos.x*matrix._13 + pos.y*matrix._23 + pos.z*matrix._33 + matrix._43;
	w = pos.x*matrix._14 + pos.y*matrix._24 + pos.z*matrix._34 + matrix._44;


	if (w < 0.001f)
		return false;

	// Normalized Device Coordinates
	D3DVECTOR NDC;
	NDC.x = x / w;
	NDC.y = y / w;
	NDC.z = z / w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	screen.z = 0.f;
	return true;
}


const char * getItem(int Item)
{
	static char buffer[40] = { 0 };
	switch (Item)
	{
	case 1: return "Lv 1 helmet";
	case 2: return "Lv 2 helmet";
	case 3: return "Lv 3 helmet";
	case 5: return "Lv 1 Armor";
	case 6: return "Lv 2 Armor";
	case 7: return "Lv 3 Armor";
	case 8: return "Lv 1 Backpack";
	case 9: return "Lv 2 Backpack";
	case 10: return "Lv 3 Backpack";
	case 19: return "Ghillie Suit";
	case 50: return "Fuel Barrel";
	case 101: return "Bandage";
	case 102: return "Med Kit";
	case 103: return "First Aid Kit";
	case 105: return "Sports Drink";
	case 106: return "Cardio Tonic";
	case 1001: return "M4A1 Rifle";
	case 1002: return "AKM Rifle";
	case 1003: return "M870 SG";
	case 1004: return "M1887 SG";
	case 1005: return "AA12 SG";
	case 1006: return "AWM SR";
	case 1007: return "Barett SR";
	case 1008: return "M249 MG";
	case 1009: return "M14EBR Rifle";
	case 1010: return "AR15 Rifle";
	case 1011: return "MP7 SMG";
	case 1012: return "PP19 SMG";
	case 1013: return "Thompson SMG";
	case 1014: return "G18C Pistol";
	case 1015: return "Desert Eagle";
	case 1017: return "Vector";
	case 1018: return "P90 SMG";
	case 1019: return "SAIGA-12 Shotgun";
	case 1020: return "WRO Hunting Rifle";
	case 1021: return "SVD SR";
	case 1022: return "M110 Sniper Rifle";
	case 1023: return "ACR Rifle";
	case 1024: return "AN94 Rifle";
	case 1025: return "MP5 SMG";
	case 1026: return "AUG Rifle";
	case 1027: return "QBU Sniper Rifle";
	case 1031: return "RPG Rocket Tube";
	case 1036: return "Kar-98";
	case 1040: return "Crossbow";
	case 1042: return "QBZ";
	case 1044: return "DSR-1 Rifle";
	case 1101: return "SG Ammo";
	case 1102: return "Rifle Ammo";
	case 1103: return "SR Ammo";
	case 1104: return "SMG Ammo";
	case 1105: return "Pistol Ammo";
	case 1106: return "RPG Ammo";
	case 1109: return "Arrow";
	case 1200: return "GLauncher";
	case 1201: return "Rifle Silencer";
	case 1202: return "SR Silence";
	case 1204: return "SMG Silencer";
	case 1211: return "Rifle Compensator";
	case 1212: return "SR Compensator";
	case 1213: return "SMG Compensator";
	case 1221: return "Rifle Flash Hider";
	case 1222: return "SR Flash Hider";
	case 1223: return "SMG Smoke Hider";
	case 1231: return "SG Choke";
	case 1241: return "Triangle Grip";
	case 1242: return "Vertical Foregrip";
	case 1251: return "Rifle QD-Mag";
	case 1252: return "Rifle EX-Mag";
	case 1253: return "Rifle EX-QD-Mag";
	case 1261: return "SR Cheek Pad";
	case 1262: return "Tactical Stock";
	case 1263: return "SG Bullet Loop";
	case 1264: return "SR EX-Mag";
	case 1265: return "SR QD-Mag";
	case 1266: return "SR EX-QD-Mag";
	case 1271: return "Red Dot Sight";
	case 1272: return "Holo-Sight";
	case 1273: return "2x Scope";
	case 1274: return "4x Scope";
	case 1275: return "8x Scope";
	case 1276: return "SMG QD-Mag";
	case 1277: return "SMG EX-Mag";
	case 1278: return "SMG QD-EX-Mag";
	case 1279: return "Collapsible Stock";
	case 1302: return "Damascus Knife";
	case 1303: return "Crowbar";
	case 1306: return "Frying Pan";
	case 1307: return "Rubber Chicken";
	case 1315: return "Lv 1 Riot-shiled";
	case 1316: return "Lv 2 Riot-shiled";
	case 1317: return "Lv 3 Riot-shiled";
	case 1401: return "Grenade";
	case 1402: return "Stun Grenade";
	case 1403: return "Smoke Grenade";
	case 1404: return "Molotov Cocktail";
	case 1405: return "Chicken Grenade";
	case 1406: return "Flash Grenade";
	default:
		memset(buffer, 0x0, sizeof(buffer));
		sprintf(buffer, "%d", Item);
		return buffer;
	}
}

int getItemLevel(int Item)
{
	switch (Item)
	{
	case 1: return 1;
	case 2: return 2;
	case 3: return 3;
	case 5: return 1;
	case 6: return 2;
	case 7: return 3;
	case 8: return 1;
	case 9: return 2;
	case 10: return 3;
	case 19: return 3;
	case 50: return 1;
	case 101: return 1;
	case 102: return 2;
	case 103: return 3;
	case 105: return 3;
	case 106: return 3;
	case 1001: return 2;
	case 1002: return 2;
	case 1003: return 1;
	case 1004: return 1;
	case 1005: return 1;
	case 1006: return 2;
	case 1007: return 3;
	case 1008: return 3;
	case 1009: return 3;
	case 1010: return 2;
	case 1011: return 2;
	case 1012: return 1;
	case 1013: return 1;
	case 1014: return 1;
	case 1015: return 1;
	case 1017: return 1;
	case 1018: return 1;
	case 1019: return 1;
	case 1020: return 1;
	case 1021: return 2;
	case 1022: return 2;
	case 1023: return 2;
	case 1024: return 3;
	case 1025: return 2;
	case 1026: return 3;
	case 1027: return 2;
	case 1031: return 1;
	case 1036: return 1;
	case 1040: return 1;
	case 1042: return 2;
	case 1044: return 1;
	case 1101: return 1;
	case 1102: return 2;
	case 1103: return 1;
	case 1104: return 1;
	case 1105: return 1;
	case 1106: return 1;
	case 1109: return 1;
	case 1200: return 1;
	case 1201: return 3;
	case 1202: return 3;
	case 1204: return 1;
	case 1211: return 2;
	case 1212: return 1;
	case 1213: return 1;
	case 1221: return 1;
	case 1222: return 1;
	case 1223: return 1;
	case 1231: return 1;
	case 1241: return 1;
	case 1242: return 2;
	case 1251: return 1;
	case 1252: return 2;
	case 1253: return 3;
	case 1261: return 1;
	case 1262: return 1;
	case 1263: return 1;;
	case 1264: return 1;
	case 1265: return 1;
	case 1266: return 1;
	case 1271: return 1;
	case 1272: return 1;
	case 1273: return 2;
	case 1274: return 3;
	case 1275: return 3;
	case 1276: return 1;
	case 1277: return 1;
	case 1278: return 1;
	case 1279: return 1;
	case 1302: return 1;
	case 1303: return 1;
	case 1306: return 1;
	case 1307: return 1;
	case 1315: return 1;
	case 1316: return 1;
	case 1317: return 1;
	case 1401: return 1;
	case 1402: return 1;
	case 1403: return 1;
	case 1404: return 1;
	case 1405: return 1;
	case 1406: return 1;
	default:
		return 4;
	}
}
