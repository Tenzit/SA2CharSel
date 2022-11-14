// SA2CharSel.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include <cstdio>
#include <vector>
#include <algorithm>
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "BossTitles.h"
#include "RankVoices.h"
#include "Base.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::transform;

#pragma region Character Loader

#define altcostume 0x80u
#define altcharacter 0x40
#define charmask ~(altcostume|altcharacter)
int defaultcharacters[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
int defaultcharacters2p[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
int defaultcharacters2palt[Characters_Amy] = { Characters_Sonic | altcharacter, Characters_Shadow | altcharacter, Characters_Tails, Characters_Eggman, Characters_Knuckles | altcharacter, Characters_Rouge | altcharacter, Characters_MechTails | altcharacter, Characters_MechEggman | altcharacter };

//0 is R101, 1 is R280
enum KartStages {
	R101,
	R280
};

enum Karts {
	Kart_TailsR101,
	Kart_RougeR280,
	Kart_Sonic,
	Kart_SonicAlt = Kart_Sonic,
	Kart_Knuckles,
	Kart_KnucklesAlt = Kart_Knuckles,
	Kart_Tails,
	Kart_TailsAlt = Kart_Tails,
	Kart_Eggman,
	Kart_EggmanAlt = Kart_Eggman,
	Kart_Shadow,
	Kart_ShadowAlt = Kart_Shadow,
	Kart_Rouge,
	Kart_RougeAlt = Kart_Rouge
};

uint32_t defaultkarts[2] = { Kart_TailsR101, Kart_RougeR280 };
uint8_t defaultkartchars[2] = { Characters_Tails, Characters_Rouge };

bool disableButtons = true;

void __cdecl LoadCharacters_r()
{
	if (CurrentLevel != LevelIDs_ChaoWorld)
	{
		if ((CurrentCharacter & ~1) == Characters_Tails)
			CurrentCharacter += Characters_MechTails - Characters_Tails;
	}
	else
	{
		if ((CurrentCharacter & ~1) == Characters_MechTails)
			CurrentCharacter -= Characters_MechTails - Characters_Tails;
	}
	if (!TwoPlayerMode)
	{
		int ch = defaultcharacters[CurrentCharacter];
		CurrentCharacter = ch & charmask;
		AltCostume[1] = AltCostume[0] = ch & altcostume ? 1 : 0;
		AltCharacter[1] = AltCharacter[0] = ch & altcharacter ? 1 : 0;
	}
	else
	{
		int ch;
		if (!AltCharacter[0])
			ch = defaultcharacters2p[CurrentCharacter];
		else
			ch = defaultcharacters2palt[CurrentCharacter];
		CurrentCharacter = ch & charmask;
		AltCostume[0] = ch & altcostume ? 1 : 0;
		AltCharacter[0] = ch & altcharacter ? 1 : 0;
		if (!AltCharacter[1])
			ch = defaultcharacters2p[CurrentCharacter2P];
		else
			ch = defaultcharacters2palt[CurrentCharacter2P];
		CurrentCharacter2P = ch & charmask;
		AltCostume[1] = ch & altcostume ? 1 : 0;
		AltCharacter[1] = ch & altcharacter ? 1 : 0;
	}

		int playerNum = 0;
		int *character = &CurrentCharacter;
		int buttons = MenuButtons_Held[0];
	LoopStart:
	if (!disableButtons) {
		if (buttons & Buttons_Left)
			*character = Characters_Sonic;
		if (buttons & Buttons_Right)
			*character = Characters_Shadow;
		if (buttons & Buttons_Down)
			*character = Characters_Knuckles;
		if (buttons & Buttons_Up)
			*character = Characters_Rouge;
		if (buttons & Buttons_R)
			*character = Characters_MechTails;
		if (buttons & Buttons_L)
			*character = Characters_MechEggman;
		if (buttons & Buttons_Y)
			*character = Characters_Tails;
		if (buttons & Buttons_X)
			*character = Characters_Eggman;
		if (buttons & Buttons_B)
			AltCharacter[playerNum] ^= 1;
		if (buttons & Buttons_A)
			AltCostume[playerNum] ^= 1;
	}
	LoadAnimations(character, playerNum);
	if (playerNum == 1)
		goto end;
	playerNum++;
	buttons = MenuButtons_Held[1];
	if ((buttons & Buttons_Start) && !disableButtons)
		CurrentCharacter2P = CurrentCharacter ^ 1;
	else if (!TwoPlayerMode)
		goto end;
	character = &CurrentCharacter2P;
	goto LoopStart;
end:
	LoadEmeraldManager_r_wrapper();
}

#pragma endregion

#pragma region Boss Characters

int bosscharacters[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
void __cdecl LoadBossCharacter()
{
	int character = bosscharacters[CurrentCharacter ^ 1];
	AltCostume[1] = character & altcostume ? 1 : 0;
	AltCharacter[1] = character & altcharacter ? 1 : 0;
	character &= charmask;
	if (!disableButtons) {
		int buttons = MenuButtons_Held[1];
		if (buttons & Buttons_Left)
			character = Characters_Sonic;
		if (buttons & Buttons_Right)
			character = Characters_Shadow;
		if (buttons & Buttons_Down)
			character = Characters_Knuckles;
		if (buttons & Buttons_Up)
			character = Characters_Rouge;
		if (buttons & Buttons_R)
			character = Characters_MechTails;
		if (buttons & Buttons_L)
			character = Characters_MechEggman;
		if (buttons & Buttons_Y)
			character = Characters_Tails;
		if (buttons & Buttons_X)
			character = Characters_Eggman;
		if (buttons & Buttons_B)
			AltCharacter[1] ^= 1;
		if (buttons & Buttons_A)
			AltCostume[1] ^= 1;
	}
	switch (character)
	{
	case Characters_Sonic:
		LoadSonic(1);
		break;
	case Characters_Shadow:
		LoadShadow(1);
		break;
	case Characters_Tails:
		LoadTails(1);
		break;
	case Characters_Eggman:
		LoadEggman(1);
		break;
	case Characters_Knuckles:
		LoadKnuckles(1);
		break;
	case Characters_Rouge:
		LoadRouge(1);
		break;
	case Characters_MechTails:
		LoadMechTails(1);
		break;
	case Characters_MechEggman:
		LoadMechEggman(1);
		break;
	}
}

const void *const loc_6193F5 = (void*)0x6193F5;
__declspec(naked) void sub_6193D0()
{
	LoadBossCharacter();
	__asm
	{
		push ebx
		push esi
		push edi
		xor ebx, ebx
		jmp loc_6193F5
	}
}

const void *const loc_4C7120 = (void*)0x4C7120;
__declspec(naked) void sub_4C7100()
{
	LoadBossCharacter();
	__asm jmp loc_4C7120
}

const void *const loc_6486B2 = (void*)0x6486B2;
__declspec(naked) void sub_648690()
{
	LoadBossCharacter();
	__asm jmp loc_6486B2
}

const void *const loc_6266A2 = (void*)0x6266A2;
__declspec(naked) void sub_626680()
{
	LoadBossCharacter();
	__asm jmp loc_6266A2
}

const void *const loc_661D12 = (void*)0x661D12;
__declspec(naked) void sub_661CF0()
{
	LoadBossCharacter();
	__asm jmp loc_661D12
}

DataPointer(uint8_t, Kart_Level, 0x1D970FC);
DataPointer(uint8_t, kart_choice_array, 0x174B021);
const void *const loc_61AECB = (void*)0x61AECB;
const void *const loc_61AF61 = (void*)0x61AF61;

__declspec(naked) void sub_61AEBD()
{

	__asm {
		mov eax, dword ptr Kart_Level
		movzx eax, byte ptr [eax]
		cmp eax, R280
		jg exit_61AECB
		je label_r280_0
		mov eax, defaultkarts[4*R101]
		mov [esp + 0x30], eax
		jmp loc_61AF61
	label_r280_0:
		mov eax, defaultkarts[4*R280]
		mov [esp + 0x30], eax
		jmp loc_61AF61
	exit_61AECB:
		mov eax, [Kart_Level]
		sub eax, 1
		jmp loc_61AECB
	}
}

const void *const loc_620303 = (void*)0x620303;
const void *const loc_6202DD = (void*)0x6202DD;
const void *const loc_6200F4 = (void*)0x6200F4;
__declspec(naked) void sub_6200E5()
{
	__asm {
		mov eax, dword ptr Kart_Level
		movzx eax, byte ptr[eax]
		cmp eax, R280
		jg exit_6200F4
		je label_r280_1
		cmp dword ptr defaultkarts[4*R101], Kart_RougeR280
		jl exit_620303
		je exit_6202DD
		mov al, byte ptr defaultkartchars[R101]
		mov ebx, dword ptr kart_choice_array
		mov byte ptr [ebx], al
		xor ebx, ebx
		jmp loc_6200F4
label_r280_1:
		cmp dword ptr defaultkarts[4*R280], Kart_RougeR280
		jl exit_620303
		je exit_6202DD
		mov al, byte ptr defaultkartchars[R280]
		mov ebx, dword ptr kart_choice_array
		mov byte ptr[ebx], al
		xor ebx, ebx
exit_6200F4:
		jmp loc_6200F4
exit_620303:
		jmp loc_620303
exit_6202DD:
		jmp loc_6202DD
	}
}
#pragma endregion

#pragma region init splitscreen

static const int loc_4EB2B5 = 0x4EB2B5;
__declspec(naked) void InitSplitscreen_r()
{
	__asm
	{
		mov eax, [MenuButtons_Held]
		mov eax, [eax+4]
		and eax, Buttons_Start
		jz label
		mov dword ptr [esp+4], 2
label:
		push    ebx
		xor     ebx, ebx
		push    esi
		push    edi
		jmp loc_4EB2B5
	}
}

#pragma endregion

#pragma region mod setting stuff

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

static const unordered_map<string, uint8_t> charnamemap = {
	{ "sonic", Characters_Sonic },
	{ "shadow", Characters_Shadow },
	{ "tails", Characters_Tails },
	{ "eggman", Characters_Eggman },
	{ "knuckles", Characters_Knuckles },
	{ "rouge", Characters_Rouge },
	{ "mechtails", Characters_MechTails },
	{ "mecheggman", Characters_MechEggman },
	{ "amy", Characters_Sonic | altcharacter },
	{ "metalsonic", Characters_Shadow | altcharacter },
	{ "tikal", Characters_Knuckles | altcharacter },
	{ "chaos", Characters_Rouge | altcharacter },
	{ "chaowalker", Characters_MechTails | altcharacter },
	{ "darkchaowalker", Characters_MechEggman | altcharacter },
	{ "sonicalt", Characters_Sonic | altcostume },
	{ "shadowalt", Characters_Shadow | altcostume },
	{ "knucklesalt", Characters_Knuckles | altcostume },
	{ "rougealt", Characters_Rouge | altcostume },
	{ "mechtailsalt", Characters_MechTails | altcostume },
	{ "mecheggmanalt", Characters_MechEggman | altcostume },
	{ "tailsr101", Characters_Tails },
	{ "rouger280", Characters_Rouge },
	{ "tailsalt", Characters_Tails | altcostume }
};

static uint8_t ParseCharacterID(const string &str, Characters def)
{
	string s = trim(str);
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto ch = charnamemap.find(s);
	if (ch != charnamemap.end())
		return ch->second;
	return def;
}

static const unordered_map<string, uint8_t> charkartmap = {
	{ "tailsr101", 0 },
	{ "rouger280", 1 },
	{ "sonic", 2 },
	{ "sonicalt", 2 },
	{ "shadow", 6 },
	{ "shadowalt", 6 },
	{ "tails", 4 },
	{ "tailsalt", 4 },
	{ "eggman", 5 },
	{ "eggmanalt", 5 },
	{ "knuckles", 3 },
	{ "knucklesalt", 3},
	{ "rouge", 7 },
	{ "rougealt", 7},

};

static uint8_t ParseCharacterKart(const string &str, uint8_t def)
{
	string s = trim(str);
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto ch = charkartmap.find(s);
	if (ch != charkartmap.end())
		return ch->second;
	return def;
}

const string charnames[Characters_Amy] = { "Sonic", "Shadow", "Tails", "Eggman", "Knuckles", "Rouge", "MechTails", "MechEggman" };
const string charnamesalt[Characters_Amy] = { "Amy", "MetalSonic", "Tails", "Eggman", "Tikal", "Chaos", "ChaoWalker", "DarkChaoWalker" };
const string kartstagenames[2] = { "R101", "R280" };

#pragma endregion

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		WriteJump(LoadCharacters, LoadCharacters_r);// LoadCharacters replacement
		WriteJump((void*)0x6193D0, sub_6193D0); // Sonic vs Shadow 1
		WriteJump((void*)0x4C7100, sub_4C7100), // Sonic vs Shadow 2
		WriteJump((void*)0x648690, sub_648690), // Knuckles vs Rouge
		WriteJump((void*)0x626680, sub_626680), // Tails vs Eggman 1
		WriteJump((void*)0x661CF0, sub_661CF0), // Tails vs Eggman 2
		WriteJump((void*)0x61AEBD, sub_61AEBD), // Kart missions kart
		WriteJump((void*)0x6200E5, sub_6200E5), // Kart missions texture
		//WriteJump(InitSplitscreen, InitSplitscreen_r)

		InitBase();
		InitBossTitles();
		InitRankVoice();
		
		const IniFile *settings = new IniFile(std::string(path) + "\\config.ini");
		for (int i = 0; i < Characters_Amy; i++)
			defaultcharacters[i] = ParseCharacterID(settings->getString("1Player", charnames[i]), (Characters)i);
		for (int i = 0; i < Characters_Amy; i++)
			defaultcharacters2p[i] = ParseCharacterID(settings->getString("2Player", charnames[i]), (Characters)i);
		for (int i = 0; i < Characters_Amy; i++)
			defaultcharacters2palt[i] = ParseCharacterID(settings->getString("2Player", charnamesalt[i]), (Characters)(i | altcharacter));
		for (int i = 0; i < Characters_Amy; i++)
			bosscharacters[i] = ParseCharacterID(settings->getString("Boss", charnames[i]), (Characters)i);
		for (int i = 0; i < 2; i++) {
			defaultkarts[i] = ParseCharacterKart(settings->getString("Kart", kartstagenames[i]), defaultkarts[i]);
			defaultkartchars[i] = ParseCharacterID(settings->getString("Kart", kartstagenames[i]), (Characters)defaultkartchars[i]);
		}
		disableButtons = settings->getBool("General", "disableButtons", true);
		delete settings;
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}