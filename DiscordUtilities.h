#pragma once
#pragma comment(lib, "ArkApi.lib")
//#pragma comment(lib, "ArkShop.lib")
//#include "includes.h"
#include "API\ARK\Ark.h"
//#include "../../../MichsPlugins/ArkShop/ArkShop/Public/Points.h"
#include "Requests.h"
#include <random>

//Discord Functions
inline bool discord_enabled;
inline std::string discord_sender_name;
inline FString discord_webhook_url;
void PostToDiscord(const std::wstring log);


struct DiscordsPing
{
	const FString Tag;
	DiscordsPing(const FString Tag) : Tag(Tag) {}
};

TArray<DiscordsPing> DiscordPing;


struct DiscordsInvite
{
	const FString Word;
	DiscordsInvite(const FString Word) : Word(Word) {}
};

TArray<DiscordsInvite> DiscordInvite;



BOOL Load();
BOOL Unload();

