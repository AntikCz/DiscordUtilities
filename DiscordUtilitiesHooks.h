#pragma once
DECLARE_HOOK(AShooterPlayerController_ServerSendChatMessage_Impl, void, AShooterPlayerController*, FString*, EChatSendMode::Type);


void InitHooks()
{
	ArkApi::GetHooks().SetHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl, &AShooterPlayerController_ServerSendChatMessage_Impl_original);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl);

}
