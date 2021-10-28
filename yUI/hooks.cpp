#if 0
#include "hooks.h"

#include <GameUI.h>
#include <span>
#include <unordered_set>

#include "GameProcess.h"
#include "GameObjects.h"
#include "commands_animation.h"
#include "SafeWrite.h"
#include "GameAPI.h"
#include "nitypes.h"
#include "SimpleINILibrary.h"
#include "utility.h"
#include "main.h"

// allow first person anims to override even if their 3rd person counterpart is missing
SInt32 g_firstPersonAnimId = -1;

AnimationContext g_animationHookContext;
bool g_startedAnimation = false;
BSAnimGroupSequence* g_lastLoopSequence = nullptr;
extern bool g_fixHolster;
float g_nextAnimOffset = 0;

#if _DEBUG
std::unordered_map<NiTransformInterpolator*, std::unordered_set<BSAnimGroupSequence*>> g_interpMap;
std::unordered_set<BSAnimGroupSequence*>* GetInterpSeq(NiTransformInterpolator* i)
{
	auto pair = g_interpMap.find(i);
	if (pair == g_interpMap.end())
		return nullptr;
	return &pair->second;
}
#endif

bool __fastcall HandleAnimationChange(AnimData* animData, UInt32 animGroupId, BSAnimGroupSequence** toMorph, UInt8* basePointer)
{
	auto* toReplace = toMorph ? *toMorph : nullptr;
#if _DEBUG
	auto& groupInfo = g_animGroupInfos[animGroupId & 0xFF];
	auto* curSeq = animData->animSequence[groupInfo.sequenceType];
	//animData->noBlend120 = true;
#endif
	g_animationHookContext = AnimationContext(basePointer);

	if (animData && animData->actor)
	{
#if 0
		if (*toMorph && animData->actor->GetFullName() /*&& animData->actor != (*g_thePlayer)*/)
			Console_Print("%X %s %s", animGroupId, animData->actor->GetFullName()->name.CStr(), (*toMorph)->sequenceName);
		auto* highProcess = (Decoding::HighProcess*) animData->actor->baseProcess;
		auto* queuedIdleFlags = &highProcess->queuedIdleFlags;
		auto* currAction = &highProcess->currentAction;
#endif
		if (IsAnimGroupReload(animGroupId) && !IsLoopingReload(animGroupId))
			g_partialLoopReloadState = PartialLoopingReloadState::NotSet;
		const auto firstPerson = animData == g_thePlayer->firstPersonAnimData;
		if (firstPerson && g_firstPersonAnimId != -1)
		{
			animGroupId = g_firstPersonAnimId;
			g_firstPersonAnimId = -1;
		}
		if (const auto animResult = GetActorAnimation(animGroupId, firstPerson, animData))
		{
			auto* newAnim = LoadAnimationPath(*animResult, animData, animGroupId);
			if (newAnim)
				*toMorph = newAnim;
#if _DEBUG
			//if (animData->actor->GetFullName() /*&& animData->actor != (*g_thePlayer)*/)
			//	Console_Print("%X %s %s", animGroupId, animData->actor->GetFullName()->name.CStr(), (*toMorph)->sequenceName);
			if (newAnim)
			{
				std::span<NiControllerSequence::ControlledBlock> blocks{ newAnim->controlledBlocks, newAnim->numControlledBlocks };
				int i = 0;
				for (auto& block : blocks)
				{
					//++i;
					//if (i < 5)
					//	continue;
					if (auto* blendInterp = block.blendInterpolator)
					{
						std::span interps{ blendInterp->m_pkInterpArray, blendInterp->m_ucArraySize };
						for (auto& i : interps)
						{
							if (i.m_spInterpolator.data)
								g_interpMap[i.m_spInterpolator.data].insert(newAnim);
						}
						//blendInterp->m_cHighPriority = 55;
					}
					//if (newAnim->animGroup->GetBaseGroupID() == kAnimGroup_AimIS)
					//	block.priority = 55;
				}
			}
#endif		
		}
		else if (toReplace)
		{
			// allow non animgroupoverride anims to use custom text keys
			static std::unordered_map<std::string, AnimPath> customKeyAnims;
			if (auto iter = customKeyAnims.find(toReplace->sequenceName); iter != customKeyAnims.end())
				HandleExtraOperations(animData, toReplace, iter->second);
			else
			{
				AnimPath ctx;
				if (HandleExtraOperations(animData, toReplace, ctx))
				{
					customKeyAnims.emplace(toReplace->sequenceName, std::move(ctx));
				}
			}
		}
	}
	if (g_nextAnimOffset && *toMorph)
	{
		(*toMorph)->SetStartOffset(g_nextAnimOffset);
		g_nextAnimOffset = 0;
	}
	return true;
}

__declspec(naked) void AnimationHook()
{
	static auto fn_AnimDataContainsAnimSequence = 0x498EA0;
	static auto returnAddress = 0x4949D5;
	__asm
	{
		push ecx

		push ebp
		lea ecx, [ebp + 0x8] // BSAnimGroupSequence* toMorph
		push ecx
		mov edx, [ebp + 0xC] // animGroupId
		mov ecx, [ebp - 0x5C] // animData
		call HandleAnimationChange
		test al, al
		jz prematureReturn
		pop ecx
		call fn_AnimDataContainsAnimSequence
		jmp returnAddress
	prematureReturn:
		add esp, 4
		mov eax, 0
		mov esp, ebp
		pop ebp
		ret 0xC
	}
}

void DecreaseAttackTimer()
{
	auto* p = static_cast<Decoding::HighProcess*>(g_thePlayer->baseProcess);
	if (!g_lastLoopSequence)
		return p->ResetAttackLoopTimer(false);
	if (g_startedAnimation)
	{
		p->time1D4 = g_lastLoopSequence->endKeyTime - g_lastLoopSequence->startTime; // start time is actually curTime
		g_startedAnimation = false;
	}
	const auto oldTime = p->time1D4;
	p->DecreaseAttackLoopShootTime(g_thePlayer);
	if (p->time1D4 >= oldTime)
	{
		p->time1D4 = 0;
		g_lastLoopSequence = nullptr;
	}
}

__declspec(naked) void EndAttackLoopHook()
{
	const static auto retnAddr = 0x941E64;
	__asm
	{
		call DecreaseAttackTimer
		jmp retnAddr
	}
}

int AnimErrorLogHook(const char* fmt, ...)
{
	va_list	args;
	va_start(args, fmt);
	char buf[0x400];
	vsprintf_s(buf, sizeof buf, fmt, args);
	DebugPrint(FormatString("GAME: %s", buf));
	va_end(args);

	return 0;
}

UInt16 __fastcall LoopingReloadFixHook(AnimData* animData, void* _edx, UInt16 groupID, bool noRecurse)
{
	const auto defaultRet = [&]()
	{
		return ThisStdCall<UInt16>(0x495740, animData, groupID, noRecurse);
	};
	groupID = groupID & 0xFF; // prevent swimming animation if crouched and reloading
	auto* weaponInfo = animData->actor->baseProcess->GetWeaponInfo();
	if (!weaponInfo)
		return defaultRet();
	const auto fullGroupId = ThisStdCall<UInt16>(0x897910, animData->actor, groupID, weaponInfo, false, animData);
	if (fullGroupId == 0xFF)
		return defaultRet();
	return fullGroupId;
}

bool __fastcall IsCustomAnimKey(const char* key)
{
	const static auto customKeys = { "noBlend", "respectEndKey", "Script:", "interruptLoop", "burstFire", "respectTextKeys", "SoundPath:", "blendToReloadLoop", "scriptLine:", "replaceWithGroup:"};
	return ra::any_of(customKeys, _L(const char* key2, StartsWith(key, key2)));
}

__declspec(naked) void KeyStringCrashFixHook()
{
	const static auto retnAddr = 0x5F4454;
	const static auto skipDest = 0x5F3BF1;
	const static auto this8addr = 0x413F40;
	__asm
	{
		call this8addr
		push eax
		mov ecx, [ebp-0x3B8]
		call IsCustomAnimKey
		test al, al
		jnz skip
		pop eax
		jmp retnAddr
	skip:
		add esp, 4
		jmp skipDest
	}
}

void FixIdleAnimStrafe()
{
	SafeWriteBuf(0x9EA0C8, "\xEB\xE", 2); // jmp 0x9EA0D8
}

#define HOOK __declspec(naked) void

void __fastcall FixBlendMult()
{
	const auto mult = GetAnimMult(g_animationHookContext.animData, *g_animationHookContext.groupID);
	if (mult > 1.0f)
	{
		*g_animationHookContext.blendAmount /= mult;
#if _DEBUG
		//Console_Print("applied mult %g, %g >> %g", mult, (*g_animationHookContext.blendAmount *= mult), *g_animationHookContext.blendAmount);
#endif
	}
}

extern float g_destFrame;

void __fastcall NiControllerSequence_ApplyDestFrameHook(NiControllerSequence* sequence, void* _EDX, float fTime, bool bUpdateInterpolators)
{

	if (sequence->state == kAnimState_Inactive)
		return;
	if (g_destFrame != -FLT_MAX && sequence->kNVSEFlags & NiControllerSequence::kFlag_DestframeStartTime)
	{
		if (sequence->offset == -FLT_MAX)
		{
			sequence->offset = -fTime;
		}
		sequence->offset += g_destFrame;
		g_destFrame = -FLT_MAX;
		sequence->kNVSEFlags ^= NiControllerSequence::kFlag_DestframeStartTime;
		sequence->destFrame = -FLT_MAX; // end my suffering
	}
	ThisStdCall(0xA34BA0, sequence, fTime, bUpdateInterpolators);
}

HOOK BlendMultHook()
{
	const static auto hookedCall = 0xA328B0;
	const static auto retnAddr = 0x4951D7;
	__asm
	{
		call hookedCall
		call FixBlendMult
		jmp retnAddr
	}
}

bool __fastcall ProlongedAimFix(UInt8* basePointer)
{
	const auto keyType = *reinterpret_cast<UInt32*>(basePointer - 0xC);
	auto* actor = *reinterpret_cast<Actor**>(basePointer - 0x30);
	if (actor != g_thePlayer)
		return keyType == kAnimKeyType_LoopingSequenceOrAim;
	auto* process = g_thePlayer->GetHighProcess();
	return keyType == kAnimKeyType_Attack && !process->isAiming || keyType == kAnimKeyType_LoopingSequenceOrAim;
}

JMP_HOOK(0x8BB96A, ProlongedAimFix, 0x8BB974, {
	_A lea ecx,[ebp]
	_A call ProlongedAimFix
	_A cmp al, 1
	_A jmp retnAddr
	})

std::unordered_set<std::string> g_reloadStartBlendFixes;

bool __fastcall ShouldPlayAimAnim(UInt8* basePointer)
{
	const static std::unordered_set ids = { kAnimGroup_ReloadWStart, kAnimGroup_ReloadYStart, kAnimGroup_ReloadXStart, kAnimGroup_ReloadZStart };
	auto* anim = *reinterpret_cast<BSAnimGroupSequence**>(basePointer - 0xA0);
	const auto queuedId = *reinterpret_cast<AnimGroupID*>(basePointer - 0x30);
	const auto currentId = *reinterpret_cast<AnimGroupID*>(basePointer - 0x34);
	auto* animData = *reinterpret_cast<AnimData**>(basePointer - 0x18c);
	const auto newCondition = _L(, queuedId == 0xFF && !ids.contains(currentId));
	const auto defaultCondition = _L(, queuedId == 0xFF);
	if (!anim)
		return defaultCondition();
	if (!g_reloadStartBlendFixes.contains(anim->sequenceName))
	{
		if (IsPlayersOtherAnimData(animData) && !g_thePlayer->IsThirdPerson())
		{
			const auto seqType = GetSequenceType(anim->animGroup->groupID);
			auto* cur1stPersonAnim = g_thePlayer->firstPersonAnimData->animSequence[seqType];
			if (cur1stPersonAnim && g_reloadStartBlendFixes.contains(cur1stPersonAnim->sequenceName))
				return newCondition();
		}
		return defaultCondition();
	}
	return newCondition();
}

HOOK NoAimInReloadLoop()
{
	constexpr static auto jumpIfTrue = 0x492CB1;
	constexpr static auto jumpIfFalse = 0x492BF8;
	__asm
	{
		lea ecx, [ebp]
		call ShouldPlayAimAnim
		test al, al
		jz isFalse
		jmp jumpIfTrue
	isFalse:
		jmp jumpIfFalse
	}
}

bool LookupAnimFromMap(NiTPointerMap<AnimSequenceBase>* animMap, UInt16 groupId, AnimSequenceBase** base, AnimData* animData)
{
	// we do not want non existing 3rd anim data to try to get played as nothing gets played if it can't find it
	//if (animData == g_thePlayer->firstPersonAnimData)
	//	animData = g_thePlayer->baseProcess->GetAnimData();

	const auto findCustomAnim = [&](AnimData* animData) -> bool
	{
		if (const auto animResult = GetActorAnimation(groupId, animData == g_thePlayer->firstPersonAnimData, animData))
		{
			const auto& animPaths = animResult->parent->anims;
			auto* animPath = !animPaths.empty() ? &animPaths.at(0) : nullptr;
			if (!animPath)
				return false;
			if (const auto animCtx = LoadCustomAnimation(animPath->path, animData))
			{
				*base = animCtx->base;
				return true;
			}
		}
		if (animData->actor == g_thePlayer && animData != g_thePlayer->firstPersonAnimData && groupId != GetActorRealAnimGroup(g_thePlayer, groupId))
		{
			// hack to allow pollCondition to work correctly in first person when groupid is decayed
			GetActorAnimation(groupId, true, g_thePlayer->firstPersonAnimData);
		}
		return false;
	};
	if (findCustomAnim(animData))
		return true;
	/*if (animData->actor == g_thePlayer && g_firstPersonAnimId == -1 && findCustomAnim(g_thePlayer->firstPersonAnimData))
	{
		// allow 1st person to use anim if it exists instead of 3rd person anim which doesn't exist
		g_firstPersonAnimId = groupId;
		*base = nullptr;
	}*/
	return false;
}

UInt8* GetParentBasePtr(void* addressOfReturnAddress)
{
	auto* basePtr = static_cast<UInt8*>(addressOfReturnAddress) - 4;
	return *reinterpret_cast<UInt8**>(basePtr);
}



// attempt to fix anims that don't get loaded since they aren't in the game to begin with
template <int AnimDataOffset>
bool __fastcall NonExistingAnimHook(NiTPointerMap<AnimSequenceBase>* animMap, void* _EDX, UInt16 groupId, AnimSequenceBase** base)
{
	if ((*base = animMap->Lookup(groupId)))
		return true;
	// check kNVSE - since we don't have access to animData we'll do some stack abuse to get it
	auto* parentBasePtr = GetParentBasePtr(_AddressOfReturnAddress());
	auto* animData = *reinterpret_cast<AnimData**>(parentBasePtr + AnimDataOffset);

	if (!animData->actor)
		return false;

	if (LookupAnimFromMap(animMap, groupId, base, animData))
		return true;
	return false;
}

void __fastcall HandleOnReload(Decoding::MiddleHighProcess* process)
{
	auto* animData = process->animData;
	if (!animData || !animData->actor)
		return;
	if (auto iter = g_reloadTracker.find(animData->actor->refID); iter != g_reloadTracker.end())
	{
		auto& handler = iter->second;
		ra::for_each(handler.subscribers, _L(auto& p, p.second = true));
	}
}

__declspec(naked) void OnReloadHook()
{
	__asm
	{
		mov ecx, [ebp-0xC] // MiddleHighProcess*
		call HandleOnReload
		mov al, 1
		mov esp, ebp
		pop ebp
		ret 4
	}
}

void ApplyHooks()
{
	const auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\kNVSE.ini)";
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());
	g_logLevel = ini.GetOrCreate("General", "iConsoleLogLevel", 0, "; 0 = no console log, 1 = error console log, 2 = ALL logs go to console");

	WriteRelJump(0x4949D0, AnimationHook);
	WriteRelJump(0x5F444F, KeyStringCrashFixHook);
	WriteRelJump(0x941E4C, EndAttackLoopHook);

	if (ini.GetOrCreate("General", "bFixLoopingReloads", 1, "; see https://www.youtube.com/watch?v=Vnh2PG-D15A"))
	{
		WriteRelCall(0x8BABCA, LoopingReloadFixHook);
		WriteRelCall(0x948CEC, LoopingReloadFixHook); // part of cancelling looping reload after shooting
	}

	if (ini.GetOrCreate("General", "bFixIdleStrafing", 1, "; allow player to strafe/turn sideways mid idle animation"))
		FixIdleAnimStrafe();

	if (ini.GetOrCreate("General", "bFixBlendAnimMultipliers", 1, "; fix blend times not being affected by animation multipliers (fixes animations playing twice in 1st person when an anim multiplier is big)"))
		WriteRelJump(0x4951D2, BlendMultHook);

#if _DEBUG || IS_TRANSITION_FIX
	//if (ini.GetOrCreate("General", "bFixAimAfterShooting", 1, "; stops the player from being stuck in irons sight aim mode after shooting a weapon while aiming"))
	{
		//APPLY_JMP(ProlongedAimFix);
		//SafeWriteBuf(0x491275, "\xEB\x0B", 2);
		WriteRelCall(0xA2E251, NiControllerSequence_ApplyDestFrameHook);
	}
#endif

	WriteRelJump(0x492BF2, NoAimInReloadLoop);

#if 1
	// attempt to fix anims that don't get loaded since they aren't in the game to begin with
	WriteRelCall(0x49575B, NonExistingAnimHook<-0x10>);
	WriteRelCall(0x495965, NonExistingAnimHook<-0x10>);
	WriteRelCall(0x4959B9, NonExistingAnimHook<-0x10>);
	WriteRelCall(0x495A03, NonExistingAnimHook<-0x10>);

	WriteRelCall(0x4948E6, NonExistingAnimHook<-0x14>);
	WriteRelCall(0x49472B, NonExistingAnimHook<-0x8>);

	/* experimental */
	WriteRelCall(0x9D0E80, NonExistingAnimHook<-0x1C>);
	WriteRelCall(0x97FF06, NonExistingAnimHook<-0x8>);
	WriteRelCall(0x97FE09, NonExistingAnimHook<-0xC>);
	WriteRelCall(0x97F36D, NonExistingAnimHook<-0xC>);
	WriteRelCall(0x8B7985, NonExistingAnimHook<-0x14>);
	WriteRelCall(0x49651B, NonExistingAnimHook<-0x14>);
	WriteRelCall(0x49651B, NonExistingAnimHook<-0x8>);
	WriteRelCall(0x495DC6, NonExistingAnimHook<-0x10>);
	WriteRelCall(0x4956CE, NonExistingAnimHook<-0x14>);
	WriteRelCall(0x495630, NonExistingAnimHook<-0x14>);
	WriteRelCall(0x49431B, NonExistingAnimHook<-0xC>);
	WriteRelCall(0x493DC0, NonExistingAnimHook<-0x98>);
	WriteRelCall(0x493115, NonExistingAnimHook<-0x18C>);

	//WriteRelCall(0x490626, NonExistingAnimHook<-0x90>);
	//WriteRelCall(0x49022F, NonExistingAnimHook<-0x54>);
	//WriteRelCall(0x490066, NonExistingAnimHook<-0x54>);
	/* experimental end */

#if 0
	WriteVirtualCall(0x8961C9, static_cast<void(__fastcall*)(BaseProcess*, void*, bool, void*, AnimData*, Actor*)>([](BaseProcess*, void*, bool isEquip, void* bip, AnimData* animData, Actor*)
	{
		if (animData != g_thePlayer->firstPersonAnimData || isEquip)
		{
			ThisStdCall(0x9231D0, g_thePlayer->GetHighProcess(), isEquip, bip, animData, g_thePlayer);
			return;
		}
		ThisStdCall(0x9231D0, g_thePlayer->GetHighProcess(), isEquip, g_thePlayer->bipedAnims1st, g_thePlayer->firstPersonAnimData, g_thePlayer);
	}));
#endif
	WriteRelJump(0x91EA33, OnReloadHook);


#endif
	ini.SaveFile(iniPath.c_str(), false);
}
#endif