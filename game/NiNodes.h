#pragma once
#include <NiTypes.h>

/*** class hierarchy
 *
 *	yet again taken from rtti information
 *	ni doesn't seem to use multiple inheritance
 *
 *	thanks to the NifTools team for their work on the on-disk format
 *	thanks to netimmerse for NiObject::DumpAttributes
 *
 *	all offsets here are assuming patch 1.2 as they changed dramatically
 *	0xE8 bytes were removed from NiObjectNET, and basically everything derives from that
 *
 *	NiObject derives from NiRefObject
 *
 *	BSFaceGenMorphData - derived from NiRefObject
 *		BSFaceGenMorphDataHead
 *		BSFaceGenMorphDataHair
 *
 *	BSTempEffect - derived from NiObject
 *		BSTempEffectDecal
 *		BSTempEffectGeometryDecal
 *		BSTempEffectParticle
 *		MagicHitEffect
 *			MagicModelHitEffect
 *			MagicShaderHitEffect
 *
 *	NiDX92DBufferData - derived from NiRefObject and something else
 *		NiDX9DepthStencilBufferData
 *			NiDX9SwapChainDepthStencilBufferData
 *			NiDX9ImplicitDepthStencilBufferData
 *			NiDX9AdditionalDepthStencilBufferData
 *		NiDX9TextureBufferData
 *		NiDX9OnscreenBufferData
 *			NiDX9SwapChainBufferData
 *			NiDX9ImplicitBufferData
 *
 *	NiObject
 *		NiObjectNET
 *			NiProperty
 *				NiTexturingProperty
 *				NiVertexColorProperty
 *				NiWireframeProperty
 *				NiZBufferProperty
 *				NiMaterialProperty
 *				NiAlphaProperty
 *				NiStencilProperty
 *				NiRendererSpecificProperty
 *				NiShadeProperty
 *					BSShaderProperty
 *						SkyShaderProperty
 *						ParticleShaderProperty
 *						BSShaderLightingProperty
 *							DistantLODShaderProperty
 *							TallGrassShaderProperty
 *							BSShaderPPLightingProperty
 *								SpeedTreeShaderPPLightingProperty
 *									SpeedTreeBranchShaderProperty
 *								Lighting30ShaderProperty
 *								HairShaderProperty
 *							SpeedTreeShaderLightingProperty
 *								SpeedTreeLeafShaderProperty
 *								SpeedTreeFrondShaderProperty
 *							GeometryDecalShaderProperty
 *						PrecipitationShaderProperty
 *						BoltShaderProperty
 *						WaterShaderProperty
 *				NiSpecularProperty
 *				NiFogProperty
 *					BSFogProperty
 *				NiDitherProperty
 *			NiTexture
 *				NiDX9Direct3DTexture
 *				NiSourceTexture
 *					NiSourceCubeMap
 *				NiRenderedTexture
 *					NiRenderedCubeMap
 *			NiAVObject
 *				NiDynamicEffect
 *					NiLight
 *						NiDirectionalLight
 *						NiPointLight
 *							NiSpotLight
 *						NiAmbientLight
 *					NiTextureEffect
 *				NiNode
 *					SceneGraph
 *					BSTempNodeManager
 *					BSTempNode
 *					BSCellNode
 *					BSClearZNode
 *					BSFadeNode
 *					BSScissorNode
 *					BSTimingNode
 *					BSFaceGenNiNode
 *					NiBillboardNode
 *					NiSwitchNode
 *						NiLODNode
 *							NiBSLODNode
 *					NiSortAdjustNode
 *					NiBSPNode
 *					ShadowSceneNode
 *				NiCamera
 *					BSCubeMapCamera - RTTI data incorrect
 *					NiScreenSpaceCamera
 *				NiGeometry
 *					NiLines
 *					NiTriBasedGeom
 *						NiTriShape
 *							BSScissorTriShape
 *							NiScreenElements
 *							NiScreenGeometry
 *							TallGrassTriShape
 *						NiTriStrips
 *							TallGrassTriStrips
 *					NiParticles
 *						NiParticleSystem
 *							NiMeshParticleSystem
 *						NiParticleMeshes
 *			NiSequenceStreamHelper
 *		NiRenderer
 *			NiDX9Renderer
 *		NiPixelData
 *		NiCollisionObject
 *			NiCollisionData
 *			bhkNiCollisionObject
 *				bhkCollisionObject
 *					bhkBlendCollisionObject
 *						WeaponObject
 *						bhkBlendCollisionObjectAddRotation
 *				bhkPCollisionObject
 *					bhkSPCollisionObject
 *		NiControllerSequence
 *			BSAnimGroupSequence
 *		NiTimeController
 *			BSDoorHavokController
 *			BSPlayerDistanceCheckController
 *			NiD3DController
 *			NiControllerManager
 *			NiInterpController
 *				NiSingleInterpController
 *					NiTransformController
 *					NiPSysModifierCtlr
 *						NiPSysEmitterCtlr
 *						NiPSysModifierBoolCtlr
 *							NiPSysModifierActiveCtlr
 *						NiPSysModifierFloatCtlr
 *							NiPSysInitialRotSpeedVarCtlr
 *							NiPSysInitialRotSpeedCtlr
 *							NiPSysInitialRotAngleVarCtlr
 *							NiPSysInitialRotAngleCtlr
 *							NiPSysGravityStrengthCtlr
 *							NiPSysFieldMaxDistanceCtlr
 *							NiPSysFieldMagnitudeCtlr
 *							NiPSysFieldAttenuationCtlr
 *							NiPSysEmitterSpeedCtlr
 *							NiPSysEmitterPlanarAngleVarCtlr
 *							NiPSysEmitterPlanarAngleCtlr
 *							NiPSysEmitterLifeSpanCtlr
 *							NiPSysEmitterInitialRadiusCtlr
 *							NiPSysEmitterDeclinationVarCtlr
 *							NiPSysEmitterDeclinationCtlr
 *							NiPSysAirFieldSpreadCtlr
 *							NiPSysAirFieldInheritVelocityCtlr
 *							NiPSysAirFieldAirFrictionCtlr
 *					NiFloatInterpController
 *						NiFlipController
 *						NiAlphaController
 *						NiTextureTransformController
 *						NiLightDimmerController
 *					NiBoolInterpController
 *						NiVisController
 *					NiPoint3InterpController
 *						NiMaterialColorController
 *						NiLightColorController
 *					NiExtraDataController
 *						NiFloatsExtraDataPoint3Controller
 *						NiFloatsExtraDataController
 *						NiFloatExtraDataController
 *						NiColorExtraDataController
 *				NiMultiTargetTransformController
 *				NiGeomMorpherController
 *			bhkBlendController
 *			bhkForceController
 *			NiBSBoneLODController
 *			NiUVController
 *			NiPathController
 *			NiLookAtController
 *			NiKeyframeManager
 *			NiBoneLODController
 *			NiPSysUpdateCtlr
 *			NiPSysResetOnLoopCtlr
 *			NiFloatController
 *				NiRollController
 *		bhkRefObject
 *			bhkSerializable
 *				bhkWorld - NiRTTI has incorrect parent
 *					bhkWorldM
 *				bhkAction
 *					bhkUnaryAction
 *						bhkMouseSpringAction
 *						bhkMotorAction
 *					bhkBinaryAction
 *						bhkSpringAction
 *						bhkAngularDashpotAction
 *						bhkDashpotAction
 *				bhkWorldObject
 *					bhkPhantom
 *						bhkShapePhantom
 *							bhkSimpleShapePhantom
 *							bhkCachingShapePhantom
 *						bhkAabbPhantom
 *							bhkAvoidBox
 *					bhkEntity
 *						bhkRigidBody
 *							bhkRigidBodyT
 *				bhkConstraint
 *					bhkLimitedHingeConstraint
 *					bhkMalleableConstraint
 *					bhkBreakableConstraint
 *					bhkWheelConstraint
 *					bhkStiffSpringConstraint
 *					bhkRagdollConstraint
 *					bhkPrismaticConstraint
 *					bhkHingeConstraint
 *					bhkBallAndSocketConstraint
 *					bhkGenericConstraint
 *						bhkFixedConstraint
 *					bhkPointToPathConstraint
 *					bhkPoweredHingeConstraint
 *				bhkShape
 *					bhkTransformShape
 *					bhkSphereRepShape
 *						bhkConvexShape
 *							bhkSphereShape
 *							bhkCapsuleShape
 *							bhkBoxShape
 *							bhkTriangleShape
 *							bhkCylinderShape
 *							bhkConvexVerticesShape
 *								bhkCharControllerShape
 *							bhkConvexTransformShape
 *							bhkConvexSweepShape
 *						bhkMultiSphereShape
 *					bhkBvTreeShape
 *						bhkTriSampledHeightFieldBvTreeShape
 *						bhkMoppBvTreeShape
 *					bhkShapeCollection
 *						bhkListShape
 *						bhkPackedNiTriStripsShape
 *						bhkNiTriStripsShape
 *					bhkHeightFieldShape
 *						bhkPlaneShape
 *				bhkCharacterProxy
 *					bhkCharacterListenerArrow - no NiRTTI
 *					bhkCharacterListenerSpell - no NiRTTI
 *					bhkCharacterController - no NiRTTI
 *		NiExtraData
 *			TESObjectExtraData
 *			BSFaceGenAnimationData
 *			BSFaceGenModelExtraData
 *			BSFaceGenBaseMorphExtraData
 *			DebugTextExtraData
 *			NiStringExtraData
 *			NiFloatExtraData
 *				FadeNodeMaxAlphaExtraData
 *			BSFurnitureMarker
 *			NiBinaryExtraData
 *			BSBound
 *			NiSCMExtraData
 *			NiTextKeyExtraData
 *			NiVertWeightsExtraData
 *			bhkExtraData
 *			PArrayPoint
 *			NiIntegerExtraData
 *				BSXFlags
 *			NiFloatsExtraData
 *			NiColorExtraData
 *			NiVectorExtraData
 *			NiSwitchStringExtraData
 *			NiStringsExtraData
 *			NiIntegersExtraData
 *			NiBooleanExtraData
 *		NiAdditionalGeometryData
 *			BSPackedAdditionalGeometryData
 *		NiGeometryData
 *			NiLinesData
 *			NiTriBasedGeomData
 *				NiTriStripsData
 *					NiTriStripsDynamicData
 *				NiTriShapeData
 *					NiScreenElementsData
 *					NiTriShapeDynamicData
 *					NiScreenGeometryData
 *			NiParticlesData
 *				NiPSysData
 *					NiMeshPSysData
 *				NiParticleMeshesData
 *		NiTask
 *			BSTECreateTask
 *			NiParallelUpdateTaskManager::SignalTask
 *			NiGeomMorpherUpdateTask
 *			NiPSysUpdateTask
 *		NiSkinInstance
 *		NiSkinPartition
 *		NiSkinData
 *		NiRenderTargetGroup
 *		Ni2DBuffer
 *			NiDepthStencilBuffer
 *		NiUVData
 *		NiStringPalette
 *		NiSequence
 *		NiRotData
 *		NiPosData
 *		NiMorphData
 *		NiTransformData
 *		NiFloatData
 *		NiColorData
 *		NiBSplineData
 *		NiBSplineBasisData
 *		NiBoolData
 *		NiTaskManager
 *			NiParallelUpdateTaskManager
 *		hkPackedNiTriStripsData
 *		NiInterpolator
 *			NiBlendInterpolator
 *				NiBlendTransformInterpolator
 *				NiBlendAccumTransformInterpolator
 *				NiBlendFloatInterpolator
 *				NiBlendQuaternionInterpolator
 *				NiBlendPoint3Interpolator
 *				NiBlendColorInterpolator
 *				NiBlendBoolInterpolator
 *			NiLookAtInterpolator
 *			NiKeyBasedInterpolator
 *				NiFloatInterpolator
 *				NiTransformInterpolator
 *				NiQuaternionInterpolator
 *				NiPoint3Interpolator
 *				NiPathInterpolator
 *				NiColorInterpolator
 *				NiBoolInterpolator
 *					NiBoolTimelineInterpolator
 *			NiBSplineInterpolator
 *				NiBSplineTransformInterpolator
 *					NiBSplineCompTransformInterpolator
 *				NiBSplinePoint3Interpolator
 *					NiBSplineCompPoint3Interpolator
 *				NiBSplineFloatInterpolator
 *					NiBSplineCompFloatInterpolator
 *				NiBSplineColorInterpolator
 *					NiBSplineCompColorInterpolator
 *		NiAVObjectPalette
 *			NiDefaultAVObjectPalette
 *		BSReference
 *		BSNodeReferences
 *		NiPalette
 *		NiLODData
 *			NiRangeLODData
 *			NiScreenLODData
 *		NiPSysModifier
 *			BSWindModifier
 *			NiPSysMeshUpdateModifier
 *			NiPSysRotationModifier
 *			NiPSysEmitter
 *				NiPSysMeshEmitter
 *				NiPSysVolumeEmitter
 *					NiPSysCylinderEmitter
 *					NiPSysSphereEmitter
 *					NiPSysBoxEmitter
 *					BSPSysArrayEmitter
 *			NiPSysGravityModifier
 *			NiPSysSpawnModifier
 *			BSParentVelocityModifier
 *			NiPSysPositionModifier
 *			NiPSysGrowFadeModifier
 *			NiPSysDragModifier
 *			NiPSysColorModifier
 *			NiPSysColliderManager
 *			NiPSysBoundUpdateModifier
 *			NiPSysBombModifier
 *			NiPSysAgeDeathModifier
 *			NiPSysFieldModifier
 *				NiPSysVortexFieldModifier
 *				NiPSysTurbulenceFieldModifier
 *				NiPSysRadialFieldModifier
 *				NiPSysGravityFieldModifier
 *				NiPSysDragFieldModifier
 *				NiPSysAirFieldModifier
 *		NiPSysEmitterCtlrData
 *		NiAccumulator
 *			NiBackToFrontAccumulator
 *				NiAlphaAccumulator
 *					BSShaderAccumulator
 *		NiScreenPolygon
 *		NiScreenTexture
 *		NiPSysCollider
 *			NiPSysSphericalCollider
 *			NiPSysPlanarCollider
 *
 *	NiShader
 *		NiD3DShaderInterface
 *			NiD3DShader
 *				NiD3DDefaultShader
 *					SkyShader
 *					ShadowLightShader
 *						ParallaxShader
 *						SkinShader
 *						HairShader
 *						SpeedTreeBranchShader
 *					WaterShaderHeightMap
 *					WaterShader
 *					WaterShaderDisplacement
 *					ParticleShader
 *					TallGrassShader
 *					PrecipitationShader
 *					SpeedTreeLeafShader
 *					BoltShader
 *					Lighting30Shader
 *					GeometryDecalShader
 *					SpeedTreeFrondShader
 *					DistantLODShader
 *
 *	NiD3DShaderConstantMap
 *		NiD3DSCM_Vertex
 *		NiD3DSCM_Pixel
 *
 ****/

class NiStream;
class NiTextKeyExtraData;

class NiMemObject
{
};

// 008
class NiRefObject : public NiMemObject
{
public:
	NiRefObject();
	~NiRefObject();

	virtual void		Destructor(bool freeThis);	// 00
	virtual void		Free(void);					// 01

//	void		** _vtbl;		// 000
	UInt32		m_uiRefCount;	// 004 - name known
};

// 008
class NiObject : public NiRefObject
{
public:
	NiObject();
	~NiObject();

	virtual NiRTTI*		GetType();		// 02
	virtual NiNode*		GetAsNiNode();	// 03 
	virtual UInt32		Unk_04();		// 04
	virtual UInt32		Unk_05();		// 05
	virtual UInt32		Unk_06();		// 06
	virtual UInt32		Unk_07();		// 07
	virtual UInt32		Unk_08();		// 08
	virtual UInt32		Unk_09();		// 09
	virtual UInt32		Unk_0A();		// 0A
	virtual UInt32		Unk_0B();		// 0B
	virtual UInt32		Unk_0C();		// 0C
	virtual UInt32		Unk_0D();		// 0D
	virtual UInt32		Unk_0E();		// 0E
	virtual UInt32		Unk_0F();		// 0F
	virtual UInt32		Unk_10();		// 10
	virtual UInt32		Unk_11();		// 11
	virtual NiObject*	Copy();			// 12 (returns this, GetAsNiObject ?). Big doubt with everything below, except last which is 022
	virtual void		Load(NiStream * stream);
	virtual void		PostLoad(NiStream * stream);
	virtual void		FindNodes(NiStream * stream);	// give NiStream all of the NiNodes we own
	virtual void		Save(NiStream * stream);
	virtual bool		Compare(NiObject * obj);
	virtual void		DumpAttributes(NiTArray <char *> * dst);
	virtual void		DumpChildAttributes(NiTArray <char *> * dst);
	virtual void		Unk_1A();
	virtual void		Unk_1B(UInt32 arg);
	virtual void		Unk_1C();
	virtual void		GetType2();					// calls GetType
	virtual void		Unk_1E(UInt32 arg);
	virtual void		Unk_1F();
	virtual void		Unk_20();
	virtual void		Unk_21();
	virtual void		Unk_22();
};

class RefNiObject
{
	NiObject*	object;	// 00
};

// 018 (used to be 100, delta E8) confirmed, confirmed no virtual funcs
class NiObjectNET : public NiObject
{
public:
	NiObjectNET();
	~NiObjectNET();
	
	const char*			m_pcName;						// 008 - name known
	NiTimeController*	m_controller;					// 00C - size ok

	NiExtraData**		m_extraDataList;				// 010 - size ok
	UInt16				m_extraDataListLen;				// 014 - size ok
	UInt16				m_extraDataListCapacity;		// 016 - size ok
	// 018

	const char* GetName() const { return m_pcName ? m_pcName : "NULL"; }
	void SetName(const char* newName);
	NiExtraData* __fastcall GetExtraData(UInt32 vtbl) const;
	NiExtraData* GetExtraData2(const char* name)
	{
		return ThisCall<NiExtraData*>(0x006FF9C0, this, name);
	}

	__forceinline bool AddExtraData(NiExtraData* xData)
	{
		return ThisCall<bool>(0xA5BA40, this, xData);
	}
	void DumpExtraData();

};
static_assert(sizeof(NiObjectNET) == 0x18);

// 030
class NiTexture : public NiObjectNET
{
public:
	NiTexture();
	~NiTexture();

	virtual UInt32	GetWidth(void) = 0;
	virtual UInt32	GetHeight(void) = 0;

	// 8
	struct Str028
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	class RendererData
	{
	public:
		virtual void	Destroy(bool arg);
		virtual UInt32	GetWidth(void);
		virtual UInt32	GetHeight(void);
		virtual void	Unk_03(void);
	};

	enum
	{
		kPixelLayout_Palette8BPP = 0,
		kPixelLayout_Raw16BPP,
		kPixelLayout_Raw32BPP,
		kPixelLayout_Compressed,
		kPixelLayout_Bumpmap,
		kPixelLayout_Palette4BPP,
		kPixelLayout_Default,
	};

	enum
	{
		kMipMap_Disabled = 0,
		kMipMap_Enabled,
		kMipMap_Default,
	};

	enum
	{
		kAlpha_None = 0,
		kAlpha_Binary,	// 1bpp
		kAlpha_Smooth,	// 8bpp
		kAlpha_Default,
	};

	UInt32			pixelLayout;	// 018
	UInt32			alphaFormat;	// 01C
	UInt32			mipmapFormat;	// 020
	RendererData	* rendererData;	// 024
	NiTexture		* nextTex;		// 028 - linked list updated in ctor/dtor
	NiTexture		* prevTex;		// 02C
};

// NiDX9Direct3DTexture - not referenced

// 048
class NiSourceTexture : public NiTexture
{
public:
	NiSourceTexture();
	~NiSourceTexture();

	virtual void	Unk_15(void);
	virtual void	FreePixelData(void);
	virtual bool	Unk_17(void);

	UInt8		unk030;				// 030 - is static?
	UInt8		unk031[3];			// 031
	void		* unk034;			// 034
	const char	* fileName;			// 038
	NiObject	* pixelData;		// 03C - NiPixelData
	UInt8		loadDirectToRender;	// 040
	UInt8		persistRenderData;	// 041
	UInt8		pad042[2];			// 042
	void		* unk044;			// 044
};

// 04C
class NiSourceCubeMap : public NiSourceTexture
{
public:
	NiSourceCubeMap();
	~NiSourceCubeMap();

	UInt32	unk048;	// 048
};

// 040
class NiRenderedTexture : public NiTexture
{
public:
	NiRenderedTexture();
	~NiRenderedTexture();

	struct Str030
	{
		UInt32	pad00;
		UInt32	pad04;
		UInt32	width;
		UInt32	height;
	};

	virtual Str030 *	Unk_15(void);

	Str030	* unk030;	// 030
	UInt32	pad034;		// 034
	UInt32	pad038;		// 038
	UInt32	pad03C;		// 03C
};

// 05C
class NiRenderedCubeMap : public NiRenderedTexture
{
public:
	NiRenderedCubeMap();
	~NiRenderedCubeMap();

	UInt32		unk040;		// 040
	NiObject	* faces[6];	// 044
};

// 018
class NiSequenceStreamHelper : public NiObjectNET
{
public:
	NiSequenceStreamHelper();
	~NiSequenceStreamHelper();
};

//	name			d3dfmt   00 01 04       08       0C       10       14       18       1C 1D 20       24       28 29 2C       30       34 35 38       3C       40 41
//	R8G8B8			00000014 01 18 00000000 00000000 00000014 00000000 00000002 00000000 08 01 00000001 00000000 08 01 00000000 00000000 08 01 00000013 00000005 00 01
//	A8R8G8B8		00000015 01 20 00000001 00000000 00000015 00000000 00000002 00000000 08 01 00000001 00000000 08 01 00000000 00000000 08 01 00000003 00000000 08 01
//	X8R8G8B8		00000016 01 20 00000000 00000000 00000016 00000000 00000002 00000000 08 01 00000001 00000000 08 01 00000000 00000000 08 01 0000000E 00000005 08 01
//	R5G6B5			00000017 01 10 00000000 00000000 00000017 00000000 00000002 00000000 05 01 00000001 00000000 06 01 00000000 00000000 05 01 00000013 00000005 00 01
//	X1R5G5B5		00000018 01 10 00000000 00000000 00000018 00000000 00000002 00000000 05 01 00000001 00000000 05 01 00000000 00000000 05 01 0000000E 00000005 01 01
//	A1R5G5B5		00000019 01 10 00000001 00000000 00000019 00000000 00000002 00000000 05 01 00000001 00000000 05 01 00000000 00000000 05 01 00000003 00000000 01 01
//	A4R4G4B4		0000001A 01 10 00000001 00000000 0000001A 00000000 00000002 00000000 04 01 00000001 00000000 04 01 00000000 00000000 04 01 00000003 00000000 04 01
//	R3G3B2			0000001B 01 0A 00000000 00000000 0000001B 00000000 00000002 00000000 02 01 00000001 00000000 03 01 00000000 00000000 03 01 0000000E 00000005 02 01
//	A8				0000001C 01 08 0000000B 00000000 0000001C 00000000 00000003 00000000 08 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A8R3G3B2		0000001D 01 10 00000001 00000000 0000001D 00000000 00000002 00000000 02 01 00000001 00000000 03 01 00000000 00000000 03 01 00000003 00000000 08 01
//	X4R4G4B4		0000001E 01 10 00000000 00000000 0000001E 00000000 00000002 00000000 04 01 00000001 00000000 04 01 00000000 00000000 04 01 0000000E 00000000 04 01
//	A2B10G10R10		0000001F 01 20 00000001 00000000 0000001F 00000000 00000000 00000000 0A 01 00000001 00000000 0A 01 00000002 00000000 0A 01 00000003 00000000 02 01
//	A8B8G8R8		00000020 01 20 00000001 00000000 00000020 00000000 00000000 00000000 08 01 00000001 00000000 08 01 00000002 00000000 08 01 00000003 00000000 08 01
//	X8B8G8R8		00000021 01 20 00000000 00000000 00000021 00000000 00000000 00000000 08 01 00000001 00000000 08 01 00000002 00000000 08 01 0000000E 00000005 08 01
//	G16R16			00000022 01 20 0000000C 00000000 00000022 00000000 00000001 00000000 10 01 00000000 00000000 10 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A2R10G10B10		00000023 01 20 00000001 00000000 00000023 00000000 00000002 00000000 0A 01 00000001 00000000 0A 01 00000000 00000000 0A 01 00000003 00000000 02 01
//	A16B16G16R16	00000024 01 40 00000001 00000000 00000024 00000000 00000000 00000001 10 01 00000001 00000001 10 01 00000002 00000001 10 01 00000003 00000001 10 01
//	A8P8			00000028 01 10 0000000C 00000000 00000028 00000000 00000010 00000003 08 01 00000003 00000000 08 01 00000013 00000005 00 01 00000013 00000005 00 01
//	P8				00000029 01 08 00000002 00000000 00000029 00000000 00000010 00000003 08 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	L8				00000032 01 08 0000000B 00000000 00000032 00000000 00000009 00000000 08 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A8L8			00000033 01 10 0000000C 00000000 00000033 00000000 00000009 00000000 08 01 00000003 00000000 08 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A4L4			00000034 01 08 0000000C 00000000 00000034 00000000 00000009 00000000 04 01 00000003 00000000 04 01 00000013 00000005 00 01 00000013 00000005 00 01
//	V8U8			0000003C 01 10 00000008 00000000 0000003C 00000000 00000005 00000000 08 01 00000006 00000000 08 01 00000013 00000005 00 01 00000013 00000005 00 01
//	L6V5U5			0000003D 01 10 00000009 00000000 0000003D 00000000 00000005 00000000 05 01 00000006 00000000 05 01 00000009 00000000 06 00 00000013 00000005 00 00
//	X8L8V8U8		0000003E 01 20 00000009 00000000 0000003E 00000000 00000005 00000000 08 01 00000006 00000000 08 01 00000009 00000000 08 00 0000000E 00000005 08 00
//	Q8W8V8U8		0000003F 01 20 00000008 00000000 0000003F 00000000 00000005 00000000 08 01 00000006 00000000 08 01 00000007 00000000 08 01 00000008 00000000 08 01
//	V16U16			00000040 01 20 00000008 00000000 00000040 00000000 00000005 00000000 10 01 00000006 00000000 10 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A2W10V10U10		00000043 01 20 00000008 00000000 00000043 00000000 00000005 00000000 0A 01 00000006 00000000 0B 01 00000007 00000000 0B 01 00000013 00000005 00 01
//	D16_LOCKABLE	00000046 01 10 0000000F 00000000 00000046 00000000 00000011 00000000 10 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D32				00000047 01 20 0000000F 00000000 00000047 00000000 00000011 00000000 20 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D15S1			00000049 01 10 0000000F 00000000 00000049 00000000 00000012 00000000 01 01 00000011 00000000 0F 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D24S8			0000004B 01 20 0000000F 00000000 0000004B 00000000 00000012 00000000 08 01 00000011 00000000 18 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D24X8			0000004D 01 20 0000000F 00000000 0000004D 00000000 0000000E 00000000 08 01 00000011 00000000 18 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D24X4S4			0000004F 01 20 0000000F 00000000 0000004F 00000000 00000012 00000000 04 01 0000000E 00000000 04 01 00000011 00000000 18 01 00000013 00000005 00 01
//	D16				00000050 01 10 0000000F 00000000 00000050 00000000 00000011 00000000 10 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	L16				00000051 01 10 0000000B 00000000 00000051 00000000 00000009 00000000 10 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D32F_LOCKABLE	00000052 01 20 0000000B 00000000 00000052 00000000 0000000E 00000005 20 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D24FS8			00000053 01 20 0000000B 00000000 00000053 00000000 0000000E 00000005 20 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	Q16W16V16U16	0000006E 01 40 0000000B 00000000 0000006E 00000000 0000000E 00000005 40 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	R16F			0000006F 01 10 0000000B 00000000 0000006F 00000000 00000000 00000001 10 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	G16R16F			00000070 01 20 0000000C 00000000 00000070 00000000 00000000 00000001 10 01 00000001 00000001 10 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A16B16G16R16F	00000071 01 40 00000001 00000000 00000071 00000000 00000000 00000001 10 01 00000001 00000001 10 01 00000002 00000001 10 01 00000003 00000001 10 01
//	R32F			00000072 01 20 0000000B 00000000 00000072 00000000 00000000 00000002 20 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	G32R32F			00000073 01 40 0000000C 00000000 00000073 00000000 00000000 00000002 20 01 00000001 00000002 20 01 00000013 00000005 00 01 00000013 00000005 00 01
//	A32B32G32R32F	00000074 01 80 00000001 00000000 00000074 00000000 00000000 00000002 20 01 00000001 00000002 20 01 00000002 00000002 20 01 00000003 00000002 20 01
//	CxV8U8			00000075 01 10 0000000B 00000000 00000075 00000000 0000000E 00000005 10 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	DXT1			xxxxxxxx 01 00 00000004 00000000 xxxxxxxx 00000000 00000004 00000004 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	DXT3			xxxxxxxx 01 00 00000005 00000000 xxxxxxxx 00000000 00000004 00000004 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	DXT5			xxxxxxxx 01 00 00000006 00000000 xxxxxxxx 00000000 00000004 00000004 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01

//	invalid			xxxxxxxx 01 00 0000000B 00000000 xxxxxxxx 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	D32_LOCKABLE	00000054 01 00 0000000B 00000000 00000054 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	S8_LOCKABLE		00000055 01 00 0000000B 00000000 00000055 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	VERTEXDATA		00000064 01 00 0000000B 00000000 00000064 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	INDEX16			00000065 01 00 0000000B 00000000 00000065 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01
//	INDEX32			00000066 01 00 0000000B 00000000 00000066 00000000 0000000E 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01 00000013 00000005 00 01

// 44
struct TextureFormat
{
	enum
	{
		kFormat_RGB = 0,		// 0
		kFormat_RGBA,			// 1
		kFormat_A,				// 2
		kFormat_Unk3,			// 3
		kFormat_DXT1,			// 4
		kFormat_DXT3,			// 5
		kFormat_DXT5,			// 6
		kFormat_Unk7,			// 7
		kFormat_Bump,			// 8
		kFormat_BumpLuminance,	// 9
		kFormat_UnkA,			// A
		kFormat_Other,			// B - A8 L8 L16 D32F_LOCKABLE D24FS8 Q16W16V16U16 R16F R32F CxV8U8
		kFormat_Other2,			// C - G16R16 A8P8 A8L8 A4L4 G16R16F G32R32F
		kFormat_UnkD,			// D
		kFormat_UnkE,			// E
		kFormat_Depth,			// F
	};

	enum
	{
		kType_Blue,			// 00
		kType_Green,		// 01
		kType_Red,			// 02
		kType_Alpha,		// 03
		kType_Unk04,		// 04
		kType_BumpU,		// 05
		kType_BumpV,		// 06
		kType_Unk07,		// 07
		kType_Unk08,		// 08
		kType_Luminance,	// 09
		kType_Unk0A,		// 0A
		kType_Unk0B,		// 0B
		kType_Unk0C,		// 0C
		kType_Unk0D,		// 0D
		kType_Unused,		// 0E
		kType_Unk0F,		// 0F
		kType_PalIdx,		// 10
		kType_Depth,		// 11
		kType_Stencil,		// 12
		kType_None,			// 13
	};

	enum
	{
		kType2_Default,		// 00
		kType2_16Bit,		// 01
		kType2_32Bit,		// 02
		kType2_Palettized,	// 03
		kType2_Compressed,	// 04
		kType2_None,		// 05
	};

	UInt8	unk00;		// 00 - always 01? (checked all D3DFMT)
	UInt8	bpp;		// 01 - zero for dxt
	UInt8	pad02[2];	// 02
	UInt32	format;		// 04 - default kFormat_A (really)
	UInt32	unk08;		// 08 - always 00000000? (checked all D3DFMT)
	UInt32	d3dfmt;		// 0C
	UInt32	unk10;		// 10 - always 00000000? (checked all D3DFMT)

	struct Channel
	{
		UInt32	type;		// 0
		UInt32	type2;		// 4
		UInt8	bits;		// 8
		UInt8	unk9;		// 9 - only seen non-01 when unused (L6V5U5 X8L8V8U8)
		UInt8	padA[2];	// A
	};

	Channel	channels[4];	// 14

	void InitFromD3DFMT(UInt32 fmt);
};

// 070
class NiPixelData : public NiObject
{
public:
	NiPixelData();
	~NiPixelData();

	// face size = unk05C[mipmapLevels]
	// total size = face size * numFaces

	TextureFormat	format;		// 008
	NiRefObject		* unk04C;	// 04C
	UInt32	unk050;			// 050
	UInt32	* width;		// 054 - array for mipmaps?
	UInt32	* height;		// 058
	UInt32	* unk05C;		// 05C - sizes?
	UInt32	mipmapLevels;	// 060
	UInt32	unk064;			// 064
	UInt32	unk068;			// 068
	UInt32	numFaces;		// 06C
};

class ControlledBlock;

// 0C
class NiInterpolator : public NiObject
{
public:
	NiInterpolator();
	~NiInterpolator();

	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);

	float m_lastTime;

};

struct UpdateParams
{
	float		timePassed;
	bool		updateControllers;
	UInt8		byte05;
	UInt8		byte06;
	UInt8		byte07;
	UInt8		byte08;
	UInt8		pad09[3];

	UpdateParams() { ZeroMemory(this, sizeof(UpdateParams)); }
};

// 34
class NiTimeController : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Update(UpdateParams& updParams);
	/*98*/virtual void		SetTarget(NiNode* pTarget);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);

	UInt16				flags;				// 08
	UInt16				unk0A;				// 0A
	float				frequency;			// 0C
	float				phaseTime;			// 10
	float				lowKeyTime;			// 14
	float				highKeyTime;		// 18
	float				startTime;			// 1C
	float				lastTime;			// 20
	float				weightedLastTime;	// 24
	float				scaledTime;			// 28
	NiNode* target;			// 2C
	NiTimeController* nextCtrl;			// 30
};

class NiDefaultAVObjectPalette;

// 7C
class NiControllerManager : public NiTimeController
{
public:
	/*B4*/virtual void		Unk_2D(void);

	NiTArray<NiControllerSequence*>				sequences;		// 34
	void* ptr44;			// 44
	UInt32										unk48;			// 48
	UInt32										unk4C;			// 4C
	NiTStringPointerMap<NiControllerSequence*>	seqStrMap;		// 50	Case-sensitive!
	NiTArray<void*>* arr64;			// 64
	UInt32										unk68;			// 68
	UInt32										unk6C;			// 6C
	UInt32										unk70;			// 70
	UInt32										unk74;			// 74
	NiDefaultAVObjectPalette* defObjPlt;		// 78

	NiControllerSequence* FindSequence(const char* seqName);
};
static_assert(sizeof(NiControllerManager) == 0x7C);

class NiInterpController : public NiTimeController
{
};

class NiBlendInterpolator : public NiInterpolator
{
public:
	struct InterpArrayItem
	{
		NiPointer<NiTransformInterpolator> m_spInterpolator;
		float m_fWeight;
		float m_fNormalizedWeight;
		char m_cPriority;
		float m_fEaseSpinner;
		float m_fUpdateTime;
	};
	unsigned char m_uFlags;
	unsigned char m_ucArraySize;
	unsigned char m_ucInterpCount;
	unsigned char m_ucSingleIdx;
	unsigned char m_cHighPriority;
	unsigned char m_cNextHighPriority;
	InterpArrayItem* m_pkInterpArray;
	NiInterpolator* m_pkSingleInterpolator;
	float m_fWeightThreshold;
	float m_fSingleTime;
	float m_fHighSumOfWeights;
	float m_fNextHighSumOfWeights;
	float m_fHighEaseSpinner;
};

class NiBlendTransformInterpolator : public NiBlendInterpolator
{
};

/* 44137 */
class NiMultiTargetTransformController : NiInterpController
{
public:
	NiBlendTransformInterpolator* m_pkBlendInterps;
	NiAVObject** m_ppkTargets;
	unsigned __int16 m_usNumInterps;
};

struct NiFixedString
{
private:
	char* data;
public:
	const char* CStr() const
	{
		return data ? data : "";
	}
};


// 068
class NiControllerSequence : public NiObject
{
public:
	NiControllerSequence();
	~NiControllerSequence();
	virtual bool		Unk_23(float arg1, UInt8 arg2);

	enum
	{
		kState_Inactive = 0,
		kState_Animating,
		kState_EaseIn,
		kState_EaseOut,
		kState_TransSource,
		kState_TransDest,
		kState_MorphSource
	};

	enum
	{
		kCycle_Loop = 0,
		kCycle_Reverse,
		kCycle_Clamp,
	};

	// 10
	struct Unk014
	{
		NiRefObject	* unk00;	// 00
		NiRefObject	* unk04;	// 04
		UInt32		unk08;		// 08
		UInt8		unk0C;		// 0C
		UInt8		unk0D;		// 0D
		UInt8		pad0E[2];	// 0E
	};

	// 10
	struct Unk018
	{
		NiRefObject	* unk00;	// 00
		UInt16		unk04;		// 04
		UInt16		unk06;		// 06
		UInt16		unk08;		// 08
		UInt16		unk0A;		// 0A
		UInt16		unk0C;		// 0C
		UInt8		pad0E[2];	// 0E
	};

	struct ControlledBlock
	{
		NiTransformInterpolator* interpolator;
		NiMultiTargetTransformController* multiTargetCtrl;
		NiBlendInterpolator* blendInterpolator;
		UInt8 blendIdx;
		UInt8 priority;
		UInt8 gap0E[2];
	};

	struct IDTag
	{
		NiFixedString m_kAVObjectName;
		NiFixedString m_kPropertyType;
		NiFixedString m_kCtlrType;
		NiFixedString m_kCtlrID;
		NiFixedString m_kInterpolatorID;
	};

	enum FlagsKNVSE
	{
		kFlag_DestframeStartTime = 1
	};

	enum AnimState
	{
		kAnimState_Inactive = 0x0,
		kAnimState_Animating = 0x1,
		kAnimState_EaseIn = 0x2,
		kAnimState_EaseOut = 0x3,
		kAnimState_TransSource = 0x4,
		kAnimState_TransDest = 0x5,
		kAnimState_MorphSource = 0x6,
	};

	enum CycleType
	{
		LOOP = 0x0,
		REVERSE = 0x1,
		CLAMP = 0x2,
		MAX_CYCLE_TYPES = 0x3,
	};

	const char* sequenceName; // 8
	UInt32 numControlledBlocks; // C
	UInt32 arrayGrowBy; // 10
	NiControllerSequence::ControlledBlock* controlledBlocks; // 14
	NiControllerSequence::IDTag* IDTagArray; // 18
	float seqWeight; // 1C
	NiTextKeyExtraData* textKeyData; // 20
	CycleType cycleType;
	float frequency;
	float beginKeyTime;
	float endKeyTime;
	float lastTime;
	float weightedLastTime;
	float lastScaledTime;
	NiControllerManager* owner;
	AnimState state;
	float offset;
	float startTime;
	float endTime;
	float destFrame;
	NiControllerSequence* partnerSequence;
	const char* accumRootName;
	UInt32 node60_maybeAccumRoot;
	UInt32 kNVSEFlags; // deprecated string palette
	SInt16 curAnimNIdx;
	UInt16 wrd6A;
	UInt32 spNotesA;
	UInt16 numNotes;
	UInt8 hasHashHashAtStartOfNodeName;
	UInt8 byte73;

	bool Play();
	void SetStartOffset(float offset)
	{
		destFrame = offset;
		kNVSEFlags |= kFlag_DestframeStartTime;
	}
};
static_assert(sizeof(NiControllerSequence) == 0x74);

// 06C
class BSAnimGroupSequence : public NiControllerSequence
{
public:
	BSAnimGroupSequence();
	~BSAnimGroupSequence();

	TESAnimGroup		* animGroup;	//068

	BSAnimGroupSequence* Get3rdPersonCounterpart() const;
};
static_assert(sizeof(BSAnimGroupSequence) == 0x78);

const auto s = sizeof(BSAnimGroupSequence);

class NiNode;

enum AnimGroupID : UInt8
{
	kAnimGroup_Idle					= 0x0,
	kAnimGroup_DynamicIdle,
	kAnimGroup_SpecialIdle,
	kAnimGroup_Forward,
	kAnimGroup_Backward,
	kAnimGroup_Left,
	kAnimGroup_Right,
	kAnimGroup_FastForward,
	kAnimGroup_FastBackward,
	kAnimGroup_FastLeft,
	kAnimGroup_FastRight,
	kAnimGroup_DodgeForward,
	kAnimGroup_DodgeBack,
	kAnimGroup_DodgeLeft,
	kAnimGroup_DodgeRight,
	kAnimGroup_TurnLeft,
	kAnimGroup_TurnRight,
	kAnimGroup_Aim,
	kAnimGroup_AimUp,
	kAnimGroup_AimDown,
	kAnimGroup_AimIS,
	kAnimGroup_AimISUp,
	kAnimGroup_AimISDown,
	kAnimGroup_Holster,
	kAnimGroup_Equip,
	kAnimGroup_Unequip,
	kAnimGroup_AttackLeft,
	kAnimGroup_AttackLeftUp,
	kAnimGroup_AttackLeftDown,
	kAnimGroup_AttackLeftIS,
	kAnimGroup_AttackLeftISUp,
	kAnimGroup_AttackLeftISDown,
	kAnimGroup_AttackRight,
	kAnimGroup_AttackRightUp,
	kAnimGroup_AttackRightDown,
	kAnimGroup_AttackRightIS,
	kAnimGroup_AttackRightISUp,
	kAnimGroup_AttackRightISDown,
	kAnimGroup_Attack3,
	kAnimGroup_Attack3Up,
	kAnimGroup_Attack3Down,
	kAnimGroup_Attack3IS,
	kAnimGroup_Attack3ISUp,
	kAnimGroup_Attack3ISDown,
	kAnimGroup_Attack4,
	kAnimGroup_Attack4Up,
	kAnimGroup_Attack4Down,
	kAnimGroup_Attack4IS,
	kAnimGroup_Attack4ISUp,
	kAnimGroup_Attack4ISDown,
	kAnimGroup_Attack5,
	kAnimGroup_Attack5Up,
	kAnimGroup_Attack5Down,
	kAnimGroup_Attack5IS,
	kAnimGroup_Attack5ISUp,
	kAnimGroup_Attack5ISDown,
	kAnimGroup_Attack6,
	kAnimGroup_Attack6Up,
	kAnimGroup_Attack6Down,
	kAnimGroup_Attack6IS,
	kAnimGroup_Attack6ISUp,
	kAnimGroup_Attack6ISDown,
	kAnimGroup_Attack7,
	kAnimGroup_Attack7Up,
	kAnimGroup_Attack7Down,
	kAnimGroup_Attack7IS,
	kAnimGroup_Attack7ISUp,
	kAnimGroup_Attack7ISDown,
	kAnimGroup_Attack8,
	kAnimGroup_Attack8Up,
	kAnimGroup_Attack8Down,
	kAnimGroup_Attack8IS,
	kAnimGroup_Attack8ISUp,
	kAnimGroup_Attack8ISDown,
	kAnimGroup_AttackLoop,
	kAnimGroup_AttackLoopUp,
	kAnimGroup_AttackLoopDown,
	kAnimGroup_AttackLoopIS,
	kAnimGroup_AttackLoopISUp,
	kAnimGroup_AttackLoopISDown,
	kAnimGroup_AttackSpin,
	kAnimGroup_AttackSpinUp,
	kAnimGroup_AttackSpinDown,
	kAnimGroup_AttackSpinIS,
	kAnimGroup_AttackSpinISUp,
	kAnimGroup_AttackSpinISDown,
	kAnimGroup_AttackSpin2,
	kAnimGroup_AttackSpin2Up,
	kAnimGroup_AttackSpin2Down,
	kAnimGroup_AttackSpin2IS,
	kAnimGroup_AttackSpin2ISUp,
	kAnimGroup_AttackSpin2ISDown,
	kAnimGroup_AttackPower,
	kAnimGroup_AttackForwardPower,
	kAnimGroup_AttackBackPower,
	kAnimGroup_AttackLeftPower,
	kAnimGroup_AttackRightPower,
	kAnimGroup_AttackCustom1Power,
	kAnimGroup_AttackCustom2Power,
	kAnimGroup_AttackCustom3Power,
	kAnimGroup_AttackCustom4Power,
	kAnimGroup_AttackCustom5Power,
	kAnimGroup_PlaceMine,
	kAnimGroup_PlaceMineUp,
	kAnimGroup_PlaceMineDown,
	kAnimGroup_PlaceMineIS,
	kAnimGroup_PlaceMineISUp,
	kAnimGroup_PlaceMineISDown,
	kAnimGroup_PlaceMine2,
	kAnimGroup_PlaceMine2Up,
	kAnimGroup_PlaceMine2Down,
	kAnimGroup_PlaceMine2IS,
	kAnimGroup_PlaceMine2ISUp,
	kAnimGroup_PlaceMine2ISDown,
	kAnimGroup_AttackThrow,
	kAnimGroup_AttackThrowUp,
	kAnimGroup_AttackThrowDown,
	kAnimGroup_AttackThrowIS,
	kAnimGroup_AttackThrowISUp,
	kAnimGroup_AttackThrowISDown,
	kAnimGroup_AttackThrow2,
	kAnimGroup_AttackThrow2Up,
	kAnimGroup_AttackThrow2Down,
	kAnimGroup_AttackThrow2IS,
	kAnimGroup_AttackThrow2ISUp,
	kAnimGroup_AttackThrow2ISDown,
	kAnimGroup_AttackThrow3,
	kAnimGroup_AttackThrow3Up,
	kAnimGroup_AttackThrow3Down,
	kAnimGroup_AttackThrow3IS,
	kAnimGroup_AttackThrow3ISUp,
	kAnimGroup_AttackThrow3ISDown,
	kAnimGroup_AttackThrow4,
	kAnimGroup_AttackThrow4Up,
	kAnimGroup_AttackThrow4Down,
	kAnimGroup_AttackThrow4IS,
	kAnimGroup_AttackThrow4ISUp,
	kAnimGroup_AttackThrow4ISDown,
	kAnimGroup_AttackThrow5,
	kAnimGroup_AttackThrow5Up,
	kAnimGroup_AttackThrow5Down,
	kAnimGroup_AttackThrow5IS,
	kAnimGroup_AttackThrow5ISUp,
	kAnimGroup_AttackThrow5ISDown,
	kAnimGroup_Attack9,
	kAnimGroup_Attack9Up,
	kAnimGroup_Attack9Down,
	kAnimGroup_Attack9IS,
	kAnimGroup_Attack9ISUp,
	kAnimGroup_Attack9ISDown,
	kAnimGroup_AttackThrow6,
	kAnimGroup_AttackThrow6Up,
	kAnimGroup_AttackThrow6Down,
	kAnimGroup_AttackThrow6IS,
	kAnimGroup_AttackThrow6ISUp,
	kAnimGroup_AttackThrow6ISDown,
	kAnimGroup_AttackThrow7,
	kAnimGroup_AttackThrow7Up,
	kAnimGroup_AttackThrow7Down,
	kAnimGroup_AttackThrow7IS,
	kAnimGroup_AttackThrow7ISUp,
	kAnimGroup_AttackThrow7ISDown,
	kAnimGroup_AttackThrow8,
	kAnimGroup_AttackThrow8Up,
	kAnimGroup_AttackThrow8Down,
	kAnimGroup_AttackThrow8IS,
	kAnimGroup_AttackThrow8ISUp,
	kAnimGroup_AttackThrow8ISDown,
	kAnimGroup_Counter,
	kAnimGroup_stomp,
	kAnimGroup_BlockIdle,
	kAnimGroup_BlockHit,
	kAnimGroup_Recoil,
	kAnimGroup_ReloadWStart,
	kAnimGroup_ReloadXStart,
	kAnimGroup_ReloadYStart,
	kAnimGroup_ReloadZStart,
	kAnimGroup_ReloadA,
	kAnimGroup_ReloadB,
	kAnimGroup_ReloadC,
	kAnimGroup_ReloadD,
	kAnimGroup_ReloadE,
	kAnimGroup_ReloadF,
	kAnimGroup_ReloadG,
	kAnimGroup_ReloadH,
	kAnimGroup_ReloadI,
	kAnimGroup_ReloadJ,
	kAnimGroup_ReloadK,
	kAnimGroup_ReloadL,
	kAnimGroup_ReloadM,
	kAnimGroup_ReloadN,
	kAnimGroup_ReloadO,
	kAnimGroup_ReloadP,
	kAnimGroup_ReloadQ,
	kAnimGroup_ReloadR,
	kAnimGroup_ReloadS,
	kAnimGroup_ReloadW,
	kAnimGroup_ReloadX,
	kAnimGroup_ReloadY,
	kAnimGroup_ReloadZ,
	kAnimGroup_JamA,
	kAnimGroup_JamB,
	kAnimGroup_JamC,
	kAnimGroup_JamD,
	kAnimGroup_JamE,
	kAnimGroup_JamF,
	kAnimGroup_JamG,
	kAnimGroup_JamH,
	kAnimGroup_JamI,
	kAnimGroup_JamJ,
	kAnimGroup_JamK,
	kAnimGroup_JamL,
	kAnimGroup_JamM,
	kAnimGroup_JamN,
	kAnimGroup_JamO,
	kAnimGroup_JamP,
	kAnimGroup_JamQ,
	kAnimGroup_JamR,
	kAnimGroup_JamS,
	kAnimGroup_JamW,
	kAnimGroup_JamX,
	kAnimGroup_JamY,
	kAnimGroup_JamZ,
	kAnimGroup_Stagger,
	kAnimGroup_Death,
	kAnimGroup_Talking,
	kAnimGroup_PipBoy,
	kAnimGroup_JumpStart,
	kAnimGroup_JumpLoop,
	kAnimGroup_JumpLand,
	kAnimGroup_HandGrip1,
	kAnimGroup_HandGrip2,
	kAnimGroup_HandGrip3,
	kAnimGroup_HandGrip4,
	kAnimGroup_HandGrip5,
	kAnimGroup_HandGrip6,
	kAnimGroup_JumpLoopForward,
	kAnimGroup_JumpLoopBackward,
	kAnimGroup_JumpLoopLeft,
	kAnimGroup_JumpLoopRight,
	kAnimGroup_PipBoyChild,
	kAnimGroup_JumpLandForward,
	kAnimGroup_JumpLandBackward,
	kAnimGroup_JumpLandLeft,
	kAnimGroup_JumpLandRight,

	kAnimGroup_Max						// = 0x0FFF,	// Temporary until known
};

// 02C+
class TESAnimGroup : NiRefObject
{
public:
	// derived from NiRefObject
	TESAnimGroup();
	~TESAnimGroup();

	virtual void Destructor(bool arg0);

	bool IsAttack()
	{
		return ThisStdCall(0x4937E0, this);
	}

	bool IsAttackIS();
	bool IsAttackNonIS();


	bool IsAim()
	{
		auto idMinor = groupID & 0xFF;
		return idMinor >= kAnimGroup_Aim && idMinor <= kAnimGroup_AimISDown;
	}

	bool IsReload()
	{
		auto idMinor = groupID & 0xFF;
		return idMinor >= kAnimGroup_ReloadWStart && idMinor <= kAnimGroup_ReloadZ;
	}
	
	// 24
	struct AnimGroupInfo
	{
		const char* name;
		bool supportsVariants;
		UInt8 pad[3];
		UInt32 sequenceType;
		UInt32 keyType;
		UInt32 unk10;
		UInt32 unk14[4];
	};

	AnimGroupInfo* GetGroupInfo() const;
	UInt8 GetBaseGroupID() const;

	struct __declspec(align(4)) AnimGroupSound
	{
		float unk00;
		UInt8 soundID;
		UInt8 gap05[3];
		UInt32 unk08;
		TESSound* sound;
	};


	UInt8 byte08[8];
	UInt16 groupID;
	UInt8 unk12[1];
	UInt32 numKeys;
	float* keyTimes;
	NiPoint3 moveVector;
	UInt8 leftOrRight_whichFootToSwitch;
	UInt8 blend;
	UInt8 blendIn;
	UInt8 blendOut;
	UInt8 decal;
	UInt8 gap2D[3];
	char* parentRootNode;
	UInt32 numPtr38s;
	AnimGroupSound* ptr38;

	static const char* StringForAnimGroupCode(UInt32 groupCode);
	static UInt32 AnimGroupForString(const char* groupName);
};
static_assert(sizeof(TESAnimGroup) == 0x3C);

void DumpAnimGroups(void);

class NiBinaryStream
{
public:
	NiBinaryStream();
	~NiBinaryStream();

	virtual void	Destructor(bool freeMemory);		// 00
	virtual void	Unk_01(void);						// 04
	virtual void	SeekCur(SInt32 delta);				// 08
	virtual void	GetBufferSize(void);				// 0C
	virtual void	InitReadWriteProcs(bool useAlt);	// 10

//	void	** m_vtbl;		// 000
	UInt32	m_offset;		// 004
	void	* m_readProc;	// 008 - function pointer
	void	* m_writeProc;	// 00C - function pointer
};

class NiFile: public NiBinaryStream
{
public:
	NiFile();
	~NiFile();

	virtual void	Seek(void);			// 14
	virtual UInt32	GetFilename(void);	// 18
	virtual void	Unk_07(void);		// 1C

	UInt32	m_bufSize;	// 010
	UInt32	m_unk014;	// 014 - Total read in buffer
	UInt32	m_unk018;	// 018 - Consumed from buffer
	UInt32	m_unk01C;	// 01C
	void*	m_buffer;	// 020
	FILE*	m_File;		// 024
};

// 158
class BSFile: NiFile
{
public:
	BSFile();
	~BSFile();

	virtual void	Reset(void);	// 20
	virtual void	Unk_09(void);	// 24
	virtual void	Unk_0A(void);	// 28
	virtual void	Unk_0B(void);	// 2C
	virtual void	Unk_0C(void);	// 30
	virtual void	Read(void);		// 34
	virtual void	Write(void);	// 38

	UInt32		m_modeReadWriteAppend;	// 028
	UInt8		m_good;					// 02C
	UInt8		pad02D[3];				// 02D
	UInt8		m_unk030;				// 030
	UInt8		pad031[3];				// 031
	UInt32		m_unk034;				// 034
	UInt32		m_unk038;				// 038 - init'd to FFFFFFFF
	UInt32		m_unk03C;				// 038
	UInt32		m_unk040;				// 038
	char		m_path[0x104];			// 044
	UInt32		m_unk148;				// 148
	UInt32		m_unk14C;				// 14C
	UInt32		m_fileSize;				// 150
	UInt32		m_unk154;				// 154
};

//// derived from NiFile, which derives from NiBinaryStream
//// 154
//class BSFile
//{
//public:
//	BSFile();
//	~BSFile();
//
//	virtual void	Destructor(bool freeMemory);				// 00
//	virtual void	Unk_01(void);								// 04
//	virtual void	Unk_02(void);								// 08
//	virtual void	Unk_03(void);								// 0C
//	virtual void	Unk_04(void);								// 10
//	virtual void	DumpAttributes(NiTArray <char *> * dst);	// 14
//	virtual UInt32	GetSize(void);								// 18
//	virtual void	Unk_07(void);								// 1C
//	virtual void	Unk_08(void);								// 20
//	virtual void	Unk_09(void);								// 24
//	virtual void	Unk_0A(void);								// 28
//	virtual void	Unk_0B(void);								// 2C
//	virtual void	Unk_0C(void);								// 30
//	virtual void	Unk_Read(void);								// 34
//	virtual void	Unk_Write(void);							// 38
//
////	void	** m_vtbl;		// 000
//	void	* m_readProc;	// 004 - function pointer
//	void	* m_writeProc;	// 008 - function pointer
//	UInt32	m_bufSize;		// 00C
//	UInt32	m_unk010;		// 010 - init'd to m_bufSize
//	UInt32	m_unk014;		// 014
//	void	* m_buf;		// 018
//	FILE	* m_file;		// 01C
//	UInt32	m_writeAccess;	// 020
//	UInt8	m_good;			// 024
//	UInt8	m_pad025[3];	// 025
//	UInt8	m_unk028;		// 028
//	UInt8	m_pad029[3];	// 029
//	UInt32	m_unk02C;		// 02C
//	UInt32	m_pos;			// 030
//	UInt32	m_unk034;		// 034
//	UInt32	m_unk038;		// 038
//	char	m_path[0x104];	// 03C
//	UInt32	m_unk140;		// 140
//	UInt32	m_unk144;		// 144
//	UInt32	m_pos2;			// 148 - used if m_pos is 0xFFFFFFFF
//	UInt32	m_unk14C;		// 14C
//	UInt32	m_fileSize;		// 150
//};

/**** misc non-NiObjects ****/

// 30
class NiPropertyState : public NiRefObject
{
public:
	NiPropertyState();
	~NiPropertyState();

	UInt32	unk008[(0x30 - 0x08) >> 2];	// 008
};

// 20
class NiDynamicEffectState : public NiRefObject
{
public:
	NiDynamicEffectState();
	~NiDynamicEffectState();

	UInt8	unk008;		// 008
	UInt8	pad009[3];	// 009
	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
};

// name is a guess
class NiCulledGeoList
{
public:
	NiCulledGeoList();
	~NiCulledGeoList();

	NiGeometry	** m_geo;		// 00
	UInt32		m_numItems;		// 04
	UInt32		m_bufLen;		// 08
	UInt32		m_bufGrowSize;	// 0C
};

// 90
class NiCullingProcess
{
public:
	NiCullingProcess();
	~NiCullingProcess();

	virtual void	Destructor(bool freeMemory);
	virtual void	Unk_01(void * arg);
	virtual void	Cull(NiCamera * camera, NiAVObject * scene, NiCulledGeoList * culledGeo);
	virtual void	AddGeo(NiGeometry * arg);

//	void			** m_vtbl;		// 00
	UInt8			m_useAddGeoFn;	// 04 - call AddGeo when true, else just add to the list
	UInt8			pad05[3];		// 05
	NiCulledGeoList	* m_culledGeo;	// 08
};

class NiTextKey : public NiMemObject
{
public:
	float m_fTime;
	NiFixedString m_kText;
};


class NiExtraData : public NiObject
{
public:
	NiFixedString m_kName;
};

// 24
class BSBound : public NiExtraData
{
public:
	NiPoint3		centre;			// 0C
	NiPoint3		dimensions;		// 18
};

class NiTextKeyExtraData : public NiExtraData
{
public:
	unsigned int m_uiNumKeys;
	NiTextKey* m_pKeys;
};


struct NiQuatTransform
{
	NiPoint3 m_kTranslate;
	NiQuaternion m_kRotate;
	float m_fScale;
};

/* 1224 */
enum KeyType
{
	NOINTERP = 0x0,
	LINKEY = 0x1,
	BEZKEY = 0x2,
	TCBKEY = 0x3,
	EULERKEY = 0x4,
	STEPKEY = 0x5,
	NUMKEYTYPES = 0x6,
};

struct NiAnimationKey
{
	float m_fTime;
};

struct NiRotKey : NiAnimationKey
{
	NiQuaternion m_quat;
};

struct NiPosKey : NiAnimationKey
{
	NiPoint3 m_Pos;
};

struct NiFloatKey : NiAnimationKey
{
	float m_fValue;
};

class NiTransformData : public NiObject
{
public:
	unsigned __int16 m_usNumRotKeys;
	unsigned __int16 m_usNumPosKeys;
	unsigned __int16 m_usNumScaleKeys;
	KeyType m_eRotType;
	KeyType m_ePosType;
	KeyType m_eScaleType;
	unsigned __int8 m_ucRotSize;
	unsigned __int8 m_ucPosSize;
	unsigned __int8 m_ucScaleSize;
	NiRotKey* m_pkRotKeys;
	NiPosKey* m_pkPosKeys;
	NiFloatKey* m_pkScaleKeys;
};

struct NiKeyBasedInterpolator : NiInterpolator
{
};

class NiTransformInterpolator : public NiKeyBasedInterpolator
{
public:
	NiQuatTransform m_kTransformValue;
	NiPointer<NiTransformData> m_spData;
	unsigned __int16 m_usLastTransIdx;
	unsigned __int16 m_usLastRotIdx;
	unsigned __int16 m_usLastScaleIdx;
	bool bPose;
};

class NiProperty : public NiObjectNET
{
public:
	NiProperty();
	~NiProperty();

	virtual UInt32	GetPropertyType();
	virtual void	UpdateController(float arg);

	enum Type
	{
		kPropertyType_Alpha = 0,
		kPropertyType_Culling = 1,
		kPropertyType_Material = 2,
		kPropertyType_Shade = 3,
		kPropertyType_TileShader = kPropertyType_Shade,
		kPropertyType_Stencil = 4,
		kPropertyType_Texturing = 5,
		kPropertyType_Dither = 8,
		kPropertyType_Specular = 9,
		kPropertyType_VertexColor = 10,
		kPropertyType_ZBuffer = 11,
		kPropertyType_Fog = 13,
	};
};
class NiTexturingProperty : public NiProperty
{
public:
	NiTexturingProperty();
	~NiTexturingProperty();

	UInt32				unk18[6];	// 18
};