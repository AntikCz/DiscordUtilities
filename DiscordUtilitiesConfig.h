#pragma once
#pragma comment(lib, "ArkApi.lib")

// Standard headers
#include <fstream>
#include <random>

// Ark API headers
#include <API\ARK\Ark.h>
#include <Requests.h>
#include <json.hpp>


nlohmann::json PluginConfig;
bool IsAdmin = true;

void LoadConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DiscordUtilities/config.json");
	file >> PluginConfig;
	file.close();

	auto command1 = PluginConfig["TagNotifier"]["Keywords"];

	for (const auto& item1 : command1)
	{

		if (item1.is_string())
		{
			FString FStringValue = FString(item1.get<std::string>().c_str());
			DiscordPing.Add(DiscordsPing(FStringValue));
		}
	}

	for (const DiscordsPing& discordPing : DiscordPing) {
		FString Tag = discordPing.Tag;

		// DEBUG
		//std::wcout << L"Trigger words: " << *Ping << std::endl;
	}

	auto command2 = PluginConfig["DiscordInvite"]["Keywords"];

	for (const auto& item2 : command2)
	{

		if (item2.is_string())
		{
			FString FStringValue = FString(item2.get<std::string>().c_str());
			DiscordInvite.Add(DiscordsInvite(FStringValue));
		}
	}

	for (const DiscordsInvite& discordInvite : DiscordInvite) {
		FString word = discordInvite.Word;

	}
}