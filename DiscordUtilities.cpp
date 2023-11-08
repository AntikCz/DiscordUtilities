#pragma once
#include "DiscordUtilities.h"
#include "DiscordUtilitiesConfig.h"
#include "DiscordUtilitiesHooks.h"

#include <ArkApiUtils.h>
#include <macros.h>

void PostToDiscord(const std::wstring log)

{
	if (!discord_enabled || discord_webhook_url.IsEmpty())
		return;
	//Format of message sent to discord with the role ping <@&936715020350414900>
	FString msg = L"{{\"content\":\"<@&936715020350414900> ```stylus\\n{}```\",\"username\":\"{}\",\"avatar_url\":null}}";
	//Original format message
	// FString msg = L"{{\"content\":\"```stylus\\n{}```\",\"username\":\"{}\",\"avatar_url\":null}}";
	FString output = FString::Format(*msg, log, discord_sender_name);
	static_cast<AShooterGameState*>(ArkApi::GetApiUtils().GetWorld()->GameStateField())->HTTPPostRequest(discord_webhook_url, output);
}


void DiscordInvMessage(AShooterPlayerController* player)
{
	try
	{


		std::wstring msg = ArkApi::Tools::Utf8Decode(PluginConfig["DiscordInvite"]["InviteMsg"]);
		auto config_color = PluginConfig["DiscordInvite"]["MsgColor"];
		const FLinearColor color{ config_color[0], config_color[1], config_color[2], config_color[3] };

		ArkApi::GetApiUtils().SendServerMessage(player, color, msg.c_str());
		//Debug
		//std::cout << "DiscordInvMSG" << player << std::endl;
	}
	catch (const std::exception& error)
	{
		AShooterPlayerController* shooterController = static_cast<AShooterPlayerController*>(player);

		ArkApi::GetApiUtils().SendServerMessage(shooterController, FColorList::Red, "Failed to send DiscordInvMSG");

		Log::GetLog()->error(error.what());
		return;
	}

}


void _cdecl Hook_AShooterPlayerController_ServerSendChatMessage_Impl(AShooterPlayerController* _this, FString* Message, EChatSendMode::Type Mode)
{

	if (_this && Mode == EChatSendMode::Type::GlobalChat && !Message->IsEmpty())
	{
		//scan message for Tag word
		if (DiscordPing.Num() > 0)
		{
			const auto& MatchingPings = DiscordPing.FindByPredicate([Message](const DiscordsPing& DiscordPing) -> bool {
				return Message->Contains(DiscordPing.Tag);
				});
			if (MatchingPings)
			{
				FString playerName = *ArkApi::IApiUtils::GetSteamName(_this);
				FString playerMessage = *Message;

				const std::wstring log = fmt::format(TEXT("({}): {}"), *playerName, *playerMessage);

				PostToDiscord(log);
			}
		}
		//Scan message for invite word
		if (DiscordInvite.Num() > 0);
		{
			const auto& MatchingInvite = DiscordInvite.FindByPredicate([Message](const DiscordsInvite& DiscordInvite) -> bool {
				return Message->Contains(DiscordInvite.Word);
				});
			if (MatchingInvite)
			{
				DiscordInvMessage(_this);

			}
		}

	}

	AShooterPlayerController_ServerSendChatMessage_Impl_original(_this, Message, Mode);


}


void ReloadConfigCMD(APlayerController* player_controller, FString* /*unused*/, bool /*unused*/)
{
	auto* shooter_controller = static_cast<AShooterPlayerController*>(player_controller);
	if (player_controller->PlayerStateField() && player_controller->bIsAdmin()())
		try
	{
		LoadConfig();
		ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Green, "Reloaded DiscordUtilities config");
	}
	catch (const std::exception& error)
	{
		ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Red, "Failed to reload config");
		Log::GetLog()->error(error.what());
		return;
	}
	else
		return;
}


BOOL Load()
{
	Log::Get().Init("DiscordUtilities");

	try
	{
		LoadConfig();

		//Discord Functions
		const auto& discord_config = PluginConfig["TagNotifier"].value("Discord", nlohmann::json::object());
		discord_enabled = discord_config.value("Enabled", false);
		discord_sender_name = discord_config.value("SenderName", "").c_str();
		discord_webhook_url = discord_config.value("URL", "").c_str();
	}
	catch (const std::exception& error)
	{
		Log::GetLog()->error(error.what());
		throw;
	}


	COMMANDS.AddChatCommand("/DiscordUtilitiesReload", &ReloadConfigCMD);
	COMMANDS.AddConsoleCommand("DiscordUtilities.Reload", &ReloadConfigCMD);
	InitHooks();

	return TRUE;
}


BOOL Unload()
{

	COMMANDS.RemoveChatCommand("/DiscordUtilitiesReload");
	COMMANDS.RemoveConsoleCommand("DiscordUtilities.Reload");
	RemoveHooks();

	return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}