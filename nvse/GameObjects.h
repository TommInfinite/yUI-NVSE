#pragma once
#include <NiNodes.h>
#include <GameForms.h>
#include <GameExtraData.h>

class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL*	GetPersistentCell(void);		// 000

//	void	** vtbl;	// 00
};
STATIC_ASSERT(sizeof(TESChildCell) == 0x4);

class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	TESObjectREFR();
	~TESObjectREFR();

	virtual void				Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void				Unk_4F(void);
	virtual void				Unk_50(void);
	virtual void				Unk_51(void);
	virtual bool				CastShadows(void);
	virtual void				Unk_53(void);
	virtual void				Unk_54(void);
	virtual void				Unk_55(void);
	virtual void				Unk_56(void);
	virtual bool				IsObstacle();
	virtual void				Unk_58(void);
	virtual void				Unk_59(void);
	virtual void				Unk_5A(void);
	virtual void				Unk_5B(void);
	virtual void				Unk_5C(void);
	virtual void				Unk_5D(void);
	virtual void				Unk_5E(void);
	virtual TESObjectREFR*		RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 unk3, UInt32 unk4, TESObjectREFR* destRef, 
											UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void				Unk_60(void);
	virtual bool				EquipObject(TESForm* item, UInt32 count, ExtraDataList* xData, bool lockEquip);
	virtual void				Unk_62(void);	// Linked to Unequip (and or equip maybe)
	virtual void				Unk_63(void);
	virtual void				AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 Quantity);	// Needs confirmation
	virtual void				Unk_65(void);
	virtual void				Unk_66(void);
	virtual void				Unk_67(void);							// Actor: GetMagicEffectList
	virtual bool				GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32				GetActorUnk0148(void);					// result can be interchanged with baseForm, so TESForm* ?
	virtual void				SetActorUnk0148(UInt32 arg0);
	virtual void				Unk_6B(void);
	virtual void				Unk_6C(void);				// REFR: GetBSFaceGenNiNodeSkinned
	virtual void				Unk_6D(void);				// REFR: calls 006C
	virtual void				Unk_6E(void);				// MobileActor: calls 006D then NiNode::Func0040
	virtual void				Unk_6F(void);
	virtual bool				Unload3D();
	virtual void				AnimateNiNode(void);					// same in FOSE ! identical to Func0052 in OBSE which says (inits animation-related data, and more)
	virtual void				GenerateNiNode(bool arg0);				// same in FOSE !
	virtual void				Set3D(NiNode* niNode, bool unloadArt);	// same in FOSE !
	virtual NiNode *			GetNiNode_v(void);						// same in FOSE !
	virtual void				Unk_75(void);
	virtual void				Unk_76(void);
	virtual void				Unk_77(void);
	virtual void				Unk_78(void);
	virtual AnimData*			GetAnimData(void);			// 0079
	virtual ValidBip01Names*	GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names*	CallGetValidBip01Names(void);
	virtual void				SetValidBip01Names(ValidBip01Names validBip01Names);
	virtual NiPoint3*			GetPos(void);				// GetPos or GetDistance
	virtual void				Unk_7E(UInt32 arg0);
	virtual void				Unk_7F(void);
	virtual void				Unk_80(UInt32 arg0);
	virtual void				Unk_81(UInt32 arg0);
	virtual void				Unk_82(void);
	virtual NiNode*				GetProjectileNode(void);
	virtual void				Unk_84(UInt32 arg0);
	virtual UInt32				Unk_85(void);				// 0 or GetActor::Unk01AC
	virtual bool				IsCharacter();				// return false for Actor and Creature, true for character and PlayerCharacter
	virtual bool				IsCreature();
	virtual bool				IsExplosion();
	virtual bool				IsProjectile();
	virtual void				Unk_8A(void);				// SetParentCell (Interior only ?)
	virtual bool				IsDying(bool arg0);			// IsDead = HasNoHealth (baseForm health <= 0 or Flags bit23 set)
	virtual bool				GetHasKnockedState(void);
	virtual bool				Unk_8D(void);
	virtual void				Unk_8E(void);
	virtual void				Unk_8F(void);
	virtual void				Unk_90(void);

	struct RenderState
	{
		TESObjectREFR*	waterRef;		// 00
		UInt32			unk04;			// 04	0-0x13 when fully-underwater; exterior only
		float			waterLevel;		// 08
		float			unk0C;			// 0C
		UInt32			unk10;			// 10
		NiNode*			niNode14;		// 14
		NiNode*			niNode18;		// 18
	};

	struct EditorData {
		UInt32			unk00;	// 00
	};
	// 0C

	TESChildCell		childCell;				// 018

	TESSound*			loopSound;				// 01C

	TESForm*			baseForm;				// 020
	
	Float32				rotX, rotY, rotZ;		// 024 - either public or accessed via simple inline accessor common to all child classes
	Float32				posX, posY, posZ;		// 030 - seems to be private
	Float32				scale;					// 03C 

	TESObjectCELL*		parentCell;				// 040
	ExtraDataList		extraDataList;			// 044
	RenderState*		renderState;			// 064	- (05C in FOSE)

	ScriptEventList*		GetEventList() const;

	bool					IsTaken() const { return flags & kFormFlags_Taken; }
	bool					IsPersistent() const { return flags & kFormFlags_Persistent; }
	bool					IsTemporary() { return flags & kFormFlags_Temporary; }
	bool					IsDeleted() const { return flags & kFormFlags_Deleted; }
	bool					IsDestroyed() { return flags & kFormFlags_Destroyed; }

	NiPoint3*				PosVector() { return (NiPoint3*)&posX; }
	CoordXY*				PosXY() { return (CoordXY*)&posX; }
	
	bool					Update3D();
	bool					Update3D_v1c();	// Less worse version as used by some modders
	TESContainer*			GetContainer();
	bool					IsMapMarker();
	bool					GetInventoryItems(InventoryItemsMap &invItems);

	static TESObjectREFR*	Create(bool bTemp = false);

	bool					GetDisabled();
	ExtraContainerChanges*	GetOrCreateContainerChanges();
	ContChangesList*		GetContainerChangesList();
	SInt32					GetItemCount(TESForm* form);
	void					AddItemAlt(TESForm* item, UInt32 count, float condition, bool doEquip);
	bool					GetInventoryItems(UInt8 typeID);
	TESObjectCELL*			GetParentCell();
	TESWorldSpace*			GetParentWorld();
	bool					GetInSameCellOrWorld(TESObjectREFR* target);
	Float32 __vectorcall	GetDistance(TESObjectREFR* target);
	void					SetPos(NiPoint3& posVector);
	void					SetAngle(NiPoint3& rotVector);
	bool					MoveToCell(TESForm* worldOrCell, NiPoint3& posVector);
	void					DeleteReference();
	TESObjectREFR*			GetMerchantContainer();
	float					GetWaterImmersionPerc();
	bool					IsMobile();
	bool					IsGrabbable();
	bool					SetLinkedRef(TESObjectREFR* linkObj, UInt8 modIdx);
	bool					ValidForHooks();
	NiAVObject*				GetNiBlock(const char* blockName);
	NiNode* __fastcall		GetNode(const char* nodeName);
	hkpRigidBody*			GetRigidBody(const char* nodeName);
	bool					RunScriptSource(const char* sourceStr);
	ExtraLock::Data*		GetLockData();
	void					SetScale(float scale);
	bool					IsOwnedByActor(Actor* actor, bool includeFactionOwnership) { return ThisCall<bool>(0x5785E0, this, actor, includeFactionOwnership); };
	TESObjectREFR*			ResolveOwnership() { return ThisCall<TESObjectREFR*>(0x567790, this); };
	bool					IsInInterior();
	NiNode*					GetNiNode();

	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, Activate, bool, 0x00573170, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute as the last call (190 bytes)
	DEFINE_MEMBER_FN(Set3D, void, 0x0094EB40, NiNode*, bool);	// Usage Set3D(niNode, true); virtual func 0073
};
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x68);

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();

	virtual void		StartHighProcess(void);		// 090
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Unk_94(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual void		Unk_AF(void);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		Unk_BA(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Unk_BE(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess*	baseProcess;						// 068
	UInt32			unk06C;								// 06C - loaded	set to the talking actor ref when initialising ExtraTalkingActor
	Actor*			conversationActor;					// 070 - loaded
	UInt32			nextHeadTrackUpdateTimer;			// 074 - loaded
	UInt32			voiceTimer;							// 078 - loaded
	UInt8			unk07C;								// 07C - loaded
	UInt8			unk07D;								// 07D - loaded
	UInt8			unk07E;								// 07E - loaded
	UInt8			unk07F;								// 07F - loaded
	UInt8			unk080;								// 080 - loaded
	UInt8			isTalkingThroughActivator;			// 081 - loaded
	UInt8			unk082;								// 082 - cleared when starting combat on player
	UInt8			unk083;								// 083 - loaded
	UInt8			unk084;								// 084 - loaded
	UInt8			unk085;								// 085 - loaded
	UInt8			extraInfoGeneralTopicDataFlags;		// 086 - loaded
	UInt8			unk087;								// 087	Init'd to the inverse of NoLowLevelProcessing
};
STATIC_ASSERT(sizeof(MobileObject) == 0x088);

class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	enum State
	{
		kCastState_Inactive = 0,
		kCastState_Aim = 1,
		kCastState_Cast = 2,
		kCastState_ReleaseCast = 3,
		kCastState_FindTargets = 4,
		kCastState_SpellDisabled = 5,
		kCastState_AlreadyCasting = 6,
		kCastState_CannotCast = 7
	};

	virtual void			AddAbility(SpellItem* splItem, bool arg2);															// 000
	virtual void			AddAddiction(SpellItem* splItem, bool arg2);														// 004
	virtual void			AddEffect(SpellItem* splItem, bool arg2);															// 008
	virtual void			CastSpell(MagicItem* spell, bool arg2, MagicTarget* target, float arg4, bool arg5);/*00C*/
	virtual void			AddDisease(SpellItem* splItem, MagicTarget* target, bool arg3);/*010*/
	virtual void			AddFormEffects(MagicItem* magItem, MagicItemForm* itemForm, bool arg3);/*014*/
	virtual MagicTarget*	PickMagicTarget();/*018*/
	virtual void			Unk_07();/*01C*/
	virtual void			Unk_08();/*020*/
	virtual void			Unk_09(UInt32 arg1, UInt32 arg2);/*024*/
	virtual bool			Unk_0A(UInt32 arg1, float* arg2, UInt32* arg3, UInt32 arg4);/*028*/
	virtual Actor*			GetActor();/*02C*/
	virtual NiNode*			GetMagicNode();/*030*/
	virtual MagicItem*		GetMagicItem();/*034*/
	virtual bool			Unk_0E(ActiveEffect* activeEffect);/*038*/
	virtual float			Unk_0F(UInt8 arg1, float arg2);/*03C*/
	virtual void			SetMagicItem(MagicItem* spell);/*040*/
	virtual MagicTarget*	GetMagicTarget();/*044*/
	virtual void			SetMagicTarget(MagicTarget* magicTarget);/*048*/
	virtual ActiveEffect*	CreateActiveEffect(MagicItem* magicItem, EffectItem* effItem, TESForm* itemForm);				// 04C

	void*	unk004;	// 004
	State	state;	// 008
};
STATIC_ASSERT(sizeof(MagicCaster) == 0xC);

typedef tList<ActiveEffect> EffectNode;

class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	virtual void			Destructor(void);
	virtual TESObjectREFR*	GetParent(void);
	virtual EffectNode*		GetEffectList(void);

	UInt8					byt004;		// 004 
	UInt8					byt005;		// 005 
	UInt8					byt006;		// 006
	UInt8					byt007;		// 007
	tList<void*>			lst008;		// 008

	void					RemoveEffect(EffectItem *effItem);
	void					StopEffect(void* arg0, bool arg1) { ThisStdCall(0x8248E0, this, arg0, arg1); }
};
STATIC_ASSERT(sizeof(MagicTarget) == 0x10);

class hkaRaycastInterface
{
public:
	hkaRaycastInterface();
	~hkaRaycastInterface();
	virtual hkaRaycastInterface*	Destroy(bool doFree);
	virtual void					Unk_01(void* arg0);
	virtual void					Unk_02();

	// Assumed to be 0x010 bytes due to context where the vftable is used
	UInt32							unk000[(0x010 - 0x004) >> 2];	// 0000
};
STATIC_ASSERT(sizeof(hkaRaycastInterface) == 0x010);

class bhkRagdollController : public hkaRaycastInterface
{
public:
	bhkRagdollController();
	~bhkRagdollController();

	UInt32	unk000[(0x021C - 0x010) >> 2];	// 0010
	UInt8	fill021C[3];					// 021C
	bool	bool021F;						// 021F	when null assume FIK status is false
	bool	fikStatus;						// 0220
	UInt8	fill0221[3];					// 0221
};
STATIC_ASSERT(sizeof(bhkRagdollController) == 0x0224);

class ActorMover	// I need to call Func008
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		Unk_02(void);
	virtual void		Unk_03(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags(void);	// for PlayerMover, it is GetMovementFlags
		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
};

typedef std::vector<TESForm*> EquippedItemsList;

class Actor : public MobileObject
{
public:
	Actor();
	~Actor();

	virtual void		Unk_C1(void);
	virtual void		Unk_C2(void);
	virtual void		Unk_C3(void);
	virtual void		Unk_C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		Unk_C7(void);
	virtual void		Unk_C8(void);
	virtual void		Unk_C9(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		Unk_CD(void);
	virtual void		Unk_CE(void);
	virtual void		Unk_CF(void);
	virtual void		Unk_D0(void);
	virtual void		Unk_D1(void);
	virtual void		Unk_D2(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual void		Unk_D6(void);
	virtual void		Unk_D7(void);
	virtual void		Unk_D8(void);	// IsPlayerRef
	virtual void		Unk_D9(void);
	virtual void		Unk_DA(void);
	virtual void		Unk_DB(void);
	virtual void		Unk_DC(void);
	virtual void		Unk_DD(void);
	virtual void		Unk_DE(void);
	virtual void		Unk_DF(void);
	virtual void		Unk_E0(void);
	virtual void		Unk_E1(void);
	virtual void		Unk_E2(void);
	virtual void		Unk_E3(void);
	virtual void		Unk_E4(void);	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		Unk_E5(void);
	virtual void		Unk_E6(void);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		Unk_E9(void);
	virtual void		Unk_EA(void);
	virtual void		Unk_EB(void);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		Unk_EE(void);
	virtual void		Unk_EF(void);
	virtual void		Unk_F0(void);
	virtual void		Unk_F1(void);
	virtual void		Unk_F2(void);
	virtual void		Unk_F3(void);
	virtual void		Unk_F4(void);
	virtual void		Unk_F5(void);
	virtual void		Unk_F6(void);
	virtual void		Unk_F7(void);
	virtual void		Unk_F8(void);	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9(void);
	virtual void		Unk_FA(void);
	virtual void		Unk_FB(void);
	virtual void		Unk_FC(void);
	virtual void		Unk_FD(void);
	virtual void		Unk_FE(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		Unk_107(void);
	virtual void		Unk_108(void);
	virtual void		Unk_109(void);
	virtual bool		IsInCombat(void);
	virtual Actor*		GetCombatTarget(void);
	virtual void		Unk_10C(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual void		Unk_10F(void);
	virtual void		Unk_110(void);
	virtual void		Unk_111(void);
	virtual void		Unk_112(void);
	virtual void		Unk_113(void);
	virtual void		Unk_114(void);
	virtual void		Unk_115(void);
	virtual void		Unk_116(void);
	virtual void		Unk_117(void);
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		Unk_11C(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		Unk_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		Unk_122(void);	//	0122	Check for LevelUp
	virtual void		Unk_123(void);
	virtual void		Unk_124(void);
	virtual void		Unk_125(void);
	virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	virtual void		Unk_127(void);
	virtual UInt8		GetPerkRank(BGSPerk* perk, bool alt);
	virtual void		Unk_129(void);
	virtual void		Unk_12A(void);
	virtual void		Unk_12B(void);
	virtual void		Unk_12C(void);
	virtual void		Unk_12D(void);
	virtual void		Unk_12E(void);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual void		Unk_131(void);
	virtual void		Unk_132(void);
	virtual void		Unk_133(void);
	virtual void		Unk_134(void);
	virtual void		Unk_135(void);
	virtual void		Unk_136(void);
	
	MagicCaster							magicCaster;					// 088
	MagicTarget							magicTarget;					// 094
	ActorValueOwner						avOwner;						// 0A4
	CachedValuesOwner					cvOwner;						// 0A8

	bhkRagdollController*				ragDollController;				// 0AC
	bhkRagdollPenetrationUtil*			ragDollPentrationUtil;			// 0B0
	UInt32								unk0B4[(0x104-0x0b4) >> 2];		// 0B4
	UInt8								unk104;							// 104 Is in combat
	UInt8								pad105[3];						// 105
	UInt32								lifeState;						// 108 saved as byte HasHealth = 1 or 2, optionally 6
	UInt32								unk10C;							// 10C
	UInt32								animGroupId110;					// 110
	UInt32								unk114[(0x140-0x114) >> 2];		// 10B 12C is an array (of combat targets ?)
	UInt32								unk140;							// 140 looks like a flag. Bit31 used while checking if non essential can take damage
	UInt8                               unk144;							// 144
	UInt8                               unk145;							// 145 Has to do with package evaluation
	UInt8                               unk146;							// 146 Has to do with package evaluation
	UInt8                               unk147;							// 147
	UInt32								unk148;							// 148
	UInt8								unk14C;							// 14C
	UInt8								isSwimming;						// 14D
	UInt8								unk14E;							// 14E
	UInt8								unk14F;							// 14F
	UInt32								unk150;							// 150	-	if Actor and not player: 0160 receives a copy of Pos[X,Y,Z] and Rot if 0170 is null during Func011B. 0170 receive ParentCell if Interior, Worldspace otherwize
	UInt32								unk154;
	UInt32								unk158;
	UInt32								unk15C;
	UInt32								unk160;							// 160 if Actor and not player: 0160 receives a copy of Pos[X,Y,Z] and Rot if 0170 is null during Func011B
	UInt32								unk164;
	UInt32								unk168;
	UInt32								unk16C;
	UInt32								unk170;							// 170 receive ParentCell if Interior, Worldspace otherwize
	UInt32								unk174;
	Float32								flt178;
	UInt32								unk17C[(0x190 - 0x17C) >> 2];	// 17C	 
	ActorMover*							actorMover;						// 190

	UInt32								unk194[(0x1B0-0x194) >> 2];		// 184 Byt018D = PlayerTeammate, 1AC is a list (or not! I have it loaded from a byte)

	UInt8								unk1B0;							// 1B0
	UInt8								unk1B1;							// 1B1
	UInt8								unk1B2[2];						// 1B2

	void								FireWeapon();
	void								EjectFromWeapon(TESObjectWEAP* weapon);
	TESObjectWEAP*						GetWeaponForm() const;
	bool								IsAnimActionReload() const;
	TESActorBase*						GetActorBase();
	void								EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 unk3 = 1,
													bool lockEquip = false, UInt32 unk5 = 1);
	void								UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 unk3 = 1,
													bool lockEquip = false, UInt32 unk5 = 1);

	EquippedItemsList					GetEquippedItems();
	ContChangesArray					GetEquippedEntryDataList();
	ContChangesExtendArray				GetEquippedExtendDataList();
};
STATIC_ASSERT(sizeof(Actor) == 0x1B4);

class Creature : public Actor
{
public:
	Creature();
	~Creature();

	virtual void	Unk_137(void);

	UInt32			unk1B4[3];			// 1B4
};
STATIC_ASSERT(sizeof(Creature) == 0x1C0);


class Character : public Actor
{
public:
	Character();
	~Character();

	virtual void	Unk_137(void);
	virtual void	Unk_138(void);

	ValidBip01Names*	validBip01Names;	// 1B4
	float				totalArmorDR;		// 1B8
	float				totalArmorDT;		// 1BC
	UInt8				isTrespassing;		// 1C0
	UInt8				byt1C1;				// 1C1
	UInt16				unk1C2;				// 1C2
	float				unk1C4;				// 1C4
};
STATIC_ASSERT(sizeof(Character) == 0x1C8);

typedef tList<BGSQuestObjective::Target> QuestObjectiveTargets;

struct PerkRank
{
	BGSPerk*	perk;
	UInt8		rank;
	UInt8		pad05[3];
};
struct CasinoStats
{
	UInt32		casinoRefID;
	UInt32		earnings;
	UInt16		unk08;
	UInt8		gap0A[2];
};

// 9BC
class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	virtual void		Unk_139(void);
	virtual void		Unk_13A(void);

	// lotsa data

	UInt32								unk1C8[17];				// 1C8	208 could be a DialogPackage
	void*								unk20C;					// 20C
	tList<ActiveEffect>*				activeEffects;			// 210
	UInt32								unk214[2];				// 214
	void*								unk21C;					// 21C
	UInt32								unk220[8];				// 220	224 is a package of type 1C
	bool								showQuestItems;			// 240
	UInt8								byte241;				// 241
	UInt8								byte242;				// 242
	UInt8								byte243;				// 243
	float								unk244[77];				// 244	have to be a set of ActorValue
	float								permAVMods[77];			// 378	have to be a set of ActorValue
	float								flt4AC;					// 4AC
	float								actorValues4B0[77];		// 4B0	have to be a set of ActorValue
	BGSNote*							note;					// 5E4
	UInt32								unk574;					// 5E8
	ImageSpaceModifierInstanceDOF*		unk5EC;					// 5EC
	ImageSpaceModifierInstanceDOF*		unk5F0;					// 5F0
	ImageSpaceModifierInstanceDRB*		unk5F4;					// 5F4
	UInt32								unk5F8;					// 5F8
	tList<Actor>						teammates;				// 5FC
	TESObjectREFR*						lastExteriorDoor;		// 604
	UInt8								isPlayingAttackSound;	// 608
	UInt8								pad609[3];				// 609
	tList<void>*						actionList;				// 60C
	tList<void>*						casinoDataList;			// 610
	tList<void>*						caravanCards1;			// 614
	tList<void>*						caravanCards2;			// 618
	UInt32								unk61C[6];				// 61C
	void*								ptr634;					// 634	bhkMouseSpringAction when there's a grabbed reference
	TESObjectREFR*						grabbedRef;				// 638
	UInt32								unk63C;					// 63C
	UInt32								unk640;					// 640
	float								grabbedWeight;			// 644
	bool								thirdPersonCacheValue;	// 648
	UInt8								unk649;					// 649
	bool								isInThirdPerson;		// 64A	= not FirstPerson -- Gets weirdly changed as a hack and thirdPersonCacheValue stores real value
	UInt8								unk64B;					// 64B
	bool								thirdPersonConsistent;	// 64C
	UInt8								byte64D;				// 64D
	UInt8								byte64E;				// 64E
	bool								isUsingScope;			// 64F
	UInt8								byte650;				// 650
	bool								alwaysRun;				// 651
	bool								autoMove;				// 652
	UInt8								byte653;				// 653
	UInt32								sleepHours;				// 654
	UInt8								isResting;				// 658	Something to do with SleepDeprivation; see 0x969DCF
	UInt8								byte659;				// 659
	UInt8								byte65A;				// 65A
	UInt8								byte65B;				// 65B
	UInt32								unk65C[4];				// 65C
	UInt8								byte66C;				// 66C
	UInt8								byte66D;				// 66D
	UInt8								byte66E;				// 66E
	UInt8								byte66F;				// 66F
	float								worldFOV;				// 670
	float								firstPersonFOV;			// 674
	float								flt678;					// 678
	UInt32								unk67C;					// 67C
	UInt8								disabledControlFlags;	// 680 kControlFlag_xxx
	UInt8								unk0681[3];				// 681
	UInt32								unk684[2];				// 684
	ValidBip01Names*					bipedAnims1st;			// 68C
	AnimData*							firstPersonAnimData;	// 690 ExtraDataAnim::Data
	NiNode*								node1stPerson;			// 694 used as node if unk64A is true
	UInt32								unk698;					// 698
	UInt32								unk69C;					// 69C
	AnimData*							unkNotTpsAniMData;		// 6A0
	UInt32								unk6A4;					// 6A4
	TESTopic*							topic;					// 6A8
	UInt32								unk6AC[3];				// 6AC
	TESQuest*							quest;					// 6B8
	tList<BGSQuestObjective>			questObjectiveList;		// 6BC
	UInt32								unk6C4[39];				// 6C4
	TESRegion*							currentRegion;			// 760
	TESRegionList						regionsList;			// 764
	UInt32								unk770[18];				// 770
	UInt8								gameDifficulty;			// 7B8
	UInt8								pad7B9[3];				// 7B9
	bool								isHardcore;				// 7BC
	UInt8								pad7BD[3];				// 7BD
	UInt32								unk7C0[49];				// 7C0
	tList<BGSEntryPointPerkEntry>		perkEntries[74];		// 884
	tList<void>							perkRanksTM;			// AD4
	tList<BGSEntryPointPerkEntry>		perkEntriesTM[74];		// ADC
	UInt32								unkD2C[4];				// D2C
	NiObject*							unkD3C;					// D3C
	UInt32								unkD40;					// D40
	Actor*								reticleActor;			// D44
	tList<void>*						compassTargets;			// D48
	UInt32								unkD4C;					// D4C
	float								lastAmmoSwapTime;		// D50
	UInt32								unkD54[4];				// D4C
	CombatActors*						combatActors;			// D64
	UInt32								teammateCount;			// D68
	UInt32								unkD6C[5];				// D6C
	NiNode*								niNodeD80;				// D80
	UInt32								unkD84[12];				// D84
	NiNode*								niNodeDB4;				// DB4
	UInt32								unkDB8[7];				// DB8
	NiPoint3							vectorDD4;				// DD4
	NiPoint3							cameraPos;				// DE0
	void*								rigidBody;				// DEC
	bool								pcInCombat;				// DF0
	bool								pcUnseen;				// DF1
	UInt8								byteDF2;				// DF2
	UInt8								byteDF3;				// DF3
	BSSimpleArray<void>					itemChanges;			// DF4
	UInt32								unkE04[5];				// E04
	float								killCamTimer;			// E18
	float								killCamCooldown;		// E1C
	UInt8								byteE20;				// E20
	UInt8								byteE21;				// E21
	UInt8								byteE22;				// E22
	UInt8								byteE23;				// E23
	float								fltE24;					// E24
	float								counterAttackTimer;		// E28
	UInt32								unkE2C;					// E2C
	UInt32								itemDetectionTimer;		// E30
	NiNode*								ironSightNode;			// E34
	bool								noHardcoreTracking;		// E38	Appears to be unused
	bool								skipHCNeedsUpdate;		// E39
	UInt8								byteE3A;				// E3A
	UInt8								byteE3B;				// E3B
	BSSimpleArray<TESAmmo*>				hotkeyedWeaponAmmos;	// E3C
	UInt32								unkE4C;					// E4C

		// 7C4 is a byte used during Combat evaluation (Player is targetted ?), 
		// 7C6 is a boolean meaning toddler, 
		// 7C7 byte bool PCCanUsePowerArmor, Byt0E39 referenced during LoadGame
		// Used when entering FlyCam : 7E8/7EC/7F0 stores Pos, 7F0 adjusted by scaledHeight multiplied by 0698 , 7E0 stores RotZ, 7E4 RotX
		// Perks forms a list at 87C and AD4. Caravan Cards at 614 and 618. Quest Stage LogEntry at 6B0. Quest Objectives at 6BC.
		// Hardcore flag would be E38. Byte at DF0 seems to be PlayerIsInCombat
		// tList at 6C4 is cleared when there is no current quest. There is another NiNode at 069C
		// list of perk and perkRank are at 0x087C and 0x0AD4 (alt perks). 086C is cleared after equipement change.
		// D68 counts the Teammates.
		// D74: 96 bytes that are cleared when the 3D is cleared.

	bool					IsThirdPerson() { return thirdPersonConsistent; }
	UInt32					GetMovementFlags() { return actorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool					IsPlayerSwimming() { return GetMovementFlags()  >> 11 & 1; }

	static PlayerCharacter*	GetSingleton();
	bool					SetSkeletonPath(const char* newPath);
	bool					SetSkeletonPath_v1c(const char* newPath);	// Less worse version as used by some modders
	static void				UpdateHead();
	QuestObjectiveTargets*	GetCurrentQuestObjectiveTargets();
	HighProcess*			GetHighProcess();
};
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xE50);

extern PlayerCharacter* g_thePlayer;

class NonActorMagicCaster : public BSExtraData
{
public:
	MagicCaster		caster;		// 0C
};

class Explosion : public MobileObject
{
public:
	Explosion();
	~Explosion();

	UInt32				unk088;
	Float32				unk08C;		// init to 3.0
	Float32				buildTime;
	Float32				radius;
	Float32				ISRadius;
	UInt32				unk09C;
	TESObjectREFR*		object0A0;
	tList<void>			list0A4;
	Sound				sound0AC;
	Sound				sound0B8;
	void*				pointLight;
	TESObjectREFR*		source;
	union
	{
		TESObjectREFR*	object0CC;			// unused most of the time
	};
	TESObjectREFR*		object0D0;
	UInt32				unk0D4;
	UInt8				dontApplyExplosionIMOD;
	UInt8				isUnderwater;
	UInt8				byte0DA;
	UInt8				byte0DB;
	union										// 0DC
	{
		TESAmmo*				 ammo;							// unused for actors ???
		NonActorMagicCaster*	unkCaster;		
	};
	TESObjectWEAP*		weapon;			// 0E0
	UInt32				unk0E4;			// 0E4
	NiPoint3			vec0E8;			// 0E8
	NiPoint3			vec0F4;			// 0F4
	Float32				unk100;			// init to -1
};
STATIC_ASSERT(sizeof(Explosion) == 0x104);


class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile();

	tList<BGSImpactData>	impactDataList;		// 0x088
	UInt8					hasImpacted;		// 0x08C
	UInt8					pad091[3];
	NiMatrix33				nimatrix33;			// 0x094
	NiPoint3				nip3;				// 0x0B8	saved separately
	UInt32					unk0C4;				// 0x0C4	saved separately
	UInt32					projFlags;			// 0x0C8
	Float32					speedMult1;			// 0x0CC
	Float32					speedMult2;			// 0x0D0
	Float32					range;				// 0x0D4
	Float32					elapsedTime;		// 0x0D8
	Float32					hitDamage;			// 0x0DC
	Float32					fadeTimePercent;	// 0x0E0
	Float32					detonationTime;		// 0x0E4
	Float32					mineBlinkTimer;		// 0x0E8
	Float32					spreadz;			// 0x0EC
	Float32					spreadx;			// 0x0F0
	union										// 0x0F4
	{
		TESAmmo*			ammo;				
		Float32				weaponHealthPercent;// set but unused
	};
	TESObjectWEAP*			weapon;				// 0x0F8
	TESObjectREFR*			source;				// 0x0FC
	TESObjectREFR*			target;				// 0x100 live grenade target
	NiPoint3				pt104;				// 0x104
	UInt32					distTravelled;		// 0x110
	void*					light;				// 0x114
	UInt8					byte118;			// 0x118
	UInt8					pad119[3];			// 0x119
	void*					node11C;			// 0x11C
	UInt32					unk120;				// 0x120
	Float32					flt124;				// 0x124
	Sound					passPlayerSound;	// 0x128
	Sound					countDownSound;		// 0x134
	UInt32					unk140;				// 0x140
	ContChangesEntry*		RILContChange;		// 0x144
	UInt8					hasPlayedPassPlayerSound;// 0x148
	UInt8					pad149[3];			// 0x149
	Float32					maxrange;
};
STATIC_ASSERT(sizeof(Projectile) == 0x150);

struct ParentSpaceNode;
struct TeleportLink
{
	TESObjectREFR*	door;
	UInt32			unk04[3];
};

struct QuestTarget
{
	BSSimpleArray<ParentSpaceNode*>		parentSpaceNodes;
	BSSimpleArray<TeleportLink*>		teleportLinks;
	NiPoint3							startPos;
	NiPoint3							endPos;
};
STATIC_ASSERT(sizeof(QuestTarget) == 0x38);