//====== Copyright � 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_TF_PLAYER_H
#define C_TF_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_playeranimstate.h"
#include "c_baseplayer.h"
#include "tf_shareddefs.h"
#include "baseparticleentity.h"
#include "tf_player_shared.h"
#include "c_tf_playerclass.h"
#include "tf_item.h"
#include "props_shared.h"
#include "hintsystem.h"
#include "c_playerattachedmodel.h"
#include "iinput.h"
#include "physpropclientside.h"

#include "hl_movedata.h"

class C_MuzzleFlashModel;
class C_BaseObject;

extern ConVar tf_medigun_autoheal;
extern ConVar of_autoreload;
extern ConVar of_autoswitchweapons;
extern ConVar cl_autorezoom;
extern ConVar ofd_color_r;
extern ConVar ofd_color_g;
extern ConVar ofd_color_b;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TFPlayer : public C_BasePlayer
{
public:

	DECLARE_CLASS( C_TFPlayer, C_BasePlayer );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_TFPlayer();
	~C_TFPlayer();

	static C_TFPlayer* GetLocalTFPlayer();

	virtual void UpdateOnRemove( void );

	virtual const QAngle& GetRenderAngles();
	virtual void UpdateClientSideAnimation();
	virtual void SetDormant( bool bDormant );
	virtual void OnPreDataChanged( DataUpdateType_t updateType );
	virtual void OnDataChanged( DataUpdateType_t updateType );
	virtual void ProcessMuzzleFlashEvent();
	virtual void ValidateModelIndex( void );

	virtual Vector GetObserverCamOrigin( void );
	virtual int DrawModel( int flags );

	virtual bool CreateMove( float flInputSampleTime, CUserCmd *pCmd );

	virtual bool				IsAllowedToSwitchWeapons( void );

	virtual void ClientThink();
	CNewParticleEffect	*SetParticleEnd( CNewParticleEffect *pParticle );

	// Deal with recording
	virtual void GetToolRecordingState( KeyValues *msg );

	CTFWeaponBase *GetActiveTFWeapon( void ) const;

	virtual void Simulate( void );
	virtual void FireEvent( const Vector& origin, const QAngle& angles, int event, const char *options );

	void FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType = TF_DMG_CUSTOM_NONE );

	void ImpactWaterTrace( trace_t &trace, const Vector &vecStart );

	bool CanAttack( void );

	C_TFPlayerClass *GetPlayerClass( void )		{ return &m_PlayerClass; }
	bool IsPlayerClass( int iClass );
	virtual int GetMaxHealth( void ) const;

	virtual int GetRenderTeamNumber( void );

	bool IsWeaponLowered( void );

	void	AvoidPlayers( CUserCmd *pCmd );

	// Get the ID target entity index. The ID target is the player that is behind our crosshairs, used to
	// display the player's name.
	void UpdateIDTarget();
	int GetIDTarget() const;
	void SetForcedIDTarget( int iTarget );

	void SetAnimation( PLAYER_ANIM playerAnim );

	virtual float GetMinFOV() const;

	virtual const QAngle& EyeAngles();

	int GetBuildResources( void );

	// MATTTODO: object selection if necessary
	void SetSelectedObject( C_BaseObject *pObject ) {}

	void GetTeamColor( Color &color );

	virtual void ComputeFxBlend( void );

	// Taunts/VCDs
	virtual bool	StartSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, C_BaseEntity *pTarget );
	virtual void	CalcView( Vector &eyeOrigin, QAngle &eyeAngles, float &zNear, float &zFar, float &fov );
	bool			StartGestureSceneEvent( CSceneEventInfo *info, CChoreoScene *scene, CChoreoEvent *event, CChoreoActor *actor, CBaseEntity *pTarget );
	void			TurnOnTauntCam( void );
	void			TurnOffTauntCam( void );

	virtual void	InitPhonemeMappings();

	// Gibs.
	void InitPlayerGibs( void );
	void CreatePlayerGibs( const Vector &vecOrigin, const Vector &vecVelocity, float flImpactScale );
	void DropPartyHat( breakablepropparams_t &breakParams, Vector &vecBreakVelocity );

	int	GetObjectCount( void );
	C_BaseObject *GetObject( int index );
	C_BaseObject *GetObjectOfType( int iObjectType, int iAltMode );
	int GetNumObjects( int iObjectType, int iAltMode );

	virtual bool ShouldCollide( int collisionGroup, int contentsMask ) const;

	float GetPercentInvisible( void );
	float GetEffectiveInvisibilityLevel( void );	// takes viewer into account

	virtual void AddDecal( const Vector& rayStart, const Vector& rayEnd,
		const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS );

	virtual void CalcDeathCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual Vector GetChaseCamViewOffset( CBaseEntity *target );

	void ClientPlayerRespawn( void );

	//void				LoadMapMusic( IBaseFileSystem *pFileSystem );
	//void				PlayMapMusic( void );
	//void				ReplayMapMusic(void);
	//bool				m_bPlayingMusic;
	//char				*m_pzMusicLink;	
	//float				m_fMusicDuration;

	void CreateSaveMeEffect( void );
	void CreateChattingEffect(void);

	CGlowObject		   *m_pGlowEffect;

	virtual void	GetGlowEffectColor( float *r, float *g, float *b );

	virtual bool	IsOverridingViewmodel( void );
	virtual int		DrawOverriddenViewmodel( C_BaseViewModel *pViewmodel, int flags );

	void			SetHealer( C_TFPlayer *pHealer, float flChargeLevel );
	void			GetHealer( C_TFPlayer **pHealer, float *flChargeLevel ) { *pHealer = m_hHealer; *flChargeLevel = m_flHealerChargeLevel; }
	float			MedicGetChargeLevel( void );
	CBaseEntity		*MedicGetHealTarget( void );

	void			StartBurningSound( void );
	void			StopBurningSound( void );
	void			OnAddTeleported( void );
	void			OnRemoveTeleported( void );
	
	void			OnAddCritBoosted( void );
	void			OnRemoveCritBoosted( void );

	bool			CanShowClassMenu( void );

	void			InitializePoseParams( void );
	void			UpdateLookAt( void );

	bool			IsEnemyPlayer( void );
	void			ShowNemesisIcon( bool bShow );

	CUtlVector<EHANDLE>		*GetSpawnedGibs( void ) { return &m_hSpawnedGibs; }

	// mag count
	CUtlVector<C_FadingPhysPropClientside *> g_Mags;

	Vector 	GetClassEyeHeight( void );

	// HL2 ladder related methods
	LadderMove_t		*GetLadderMove() { return &/*m_HL2Local.*/m_LadderMove; }
	virtual void		ExitLadder();
	//virtual surfacedata_t *GetLadderSurface(const Vector &origin);

	void			ForceUpdateObjectHudState( void );

	bool			GetMedigunAutoHeal( void ){ return tf_medigun_autoheal.GetBool(); }
	bool			ShouldAutoRezoom( void ){ return cl_autorezoom.GetBool(); }
	bool			ShouldAutoReload( void ){ return of_autoreload.GetBool(); }
	bool			ShouldAutoSwitchWeapons( void ){ return of_autoswitchweapons.GetBool(); }

	CNetworkVar(bool, m_bHauling);
	bool			IsHauling( void ) { return m_bHauling; }
	void			SetHauling( bool bHauling ) { m_bHauling = bHauling; }

public:
	// Shared functions
	void			TeamFortress_SetSpeed();
	bool			HasItem( void );					// Currently can have only one item at a time.
	void			SetItem( C_TFItem *pItem );
	C_TFItem		*GetItem( void );
	bool			HasTheFlag( void );
	float			GetCritMult( void ) { return m_Shared.GetCritMult(); }

	virtual void	ItemPostFrame( void );

	void			SetOffHandWeapon( CTFWeaponBase *pWeapon );
	void			HolsterOffHandWeapon( void );

	virtual int GetSkin();

	virtual bool		Weapon_ShouldSetLast( CBaseCombatWeapon *pOldWeapon, CBaseCombatWeapon *pNewWeapon );
	virtual	bool		Weapon_Switch( C_BaseCombatWeapon *pWeapon, int viewmodelindex = 0 );

	CTFWeaponBase		*Weapon_OwnsThisID( int iWeaponID );
	CTFWeaponBase		*Weapon_GetWeaponByType( int iType );

	virtual void		GetStepSoundVelocities( float *velwalk, float *velrun );
	virtual void		SetStepSoundTime( stepsoundtimes_t iStepSoundTime, bool bWalking );

	bool	DoClassSpecialSkill( void );
	bool	CanGoInvisible( void );

	bool				IsEnemy(const C_BaseEntity *pEntity) const;

public:
	// Ragdolls.
	virtual C_BaseAnimating *BecomeRagdollOnClient();
	virtual IRagdoll		*GetRepresentativeRagdoll() const;
	EHANDLE	m_hRagdoll;
	Vector m_vecRagdollVelocity;

	// Objects
	int CanBuild( int iObjectType, int iAltMode );
	CUtlVector< CHandle<C_BaseObject> > m_aObjects;

	virtual CStudioHdr *OnNewModel( void );

	void				DisplaysHintsForTarget( C_BaseEntity *pTarget );

	// Shadows
	virtual ShadowType_t ShadowCastType( void ) ;
	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );
	virtual void GetRenderBounds( Vector& theMins, Vector& theMaxs );
	virtual bool GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const;

	CMaterialReference *GetInvulnMaterialRef( void ) { return &m_InvulnerableMaterial; }
	bool IsNemesisOfLocalPlayer();
	bool ShouldShowNemesisIcon();

	virtual	IMaterial *GetHeadLabelMaterial( void );
	
	virtual Vector GetItemTintColor( void ) { return m_vecPlayerColor; }

protected:

	void ResetFlexWeights( CStudioHdr *pStudioHdr );

private:

	void HandleTaunting( void );

	void OnPlayerClassChange( void );

	void InitInvulnerableMaterial( void );

	bool				m_bWasTaunting;
	CameraThirdData_t	m_TauntCameraData;

	QAngle				m_angTauntPredViewAngles;
	QAngle				m_angTauntEngViewAngles;

public:

	Vector				m_vecPlayerColor;	
	
	void UpdatePlayerAttachedModels( void );
	void UpdatePartyHat( void );
	void UpdateSpyMask( void );
	void UpdateWearables( void );
	void UpdateGameplayAttachments( void );
	
	bool WearsPartyHat( void );
private:

	C_TFPlayerClass		m_PlayerClass;

	// ID Target
	int					m_iIDEntIndex;
	int					m_iForcedIDTarget;

	CountdownTimer m_blinkTimer;

	CNewParticleEffect	*m_pTeleporterEffect;
	CNewParticleEffect	*m_pCritEffect;
	bool				m_bToolRecordingVisibility;

	int					m_iOldState;
	int					m_iOldSpawnCounter;

	// Healer
	CHandle<C_TFPlayer>	m_hHealer;
	float				m_flHealerChargeLevel;
	int					m_iOldHealth;

	CNetworkVar( int, m_iPlayerModelIndex );

	// Look At
	/*
	int m_headYawPoseParam;
	int m_headPitchPoseParam;
	float m_headYawMin;
	float m_headYawMax;
	float m_headPitchMin;
	float m_headPitchMax;
	float m_flLastBodyYaw;
	float m_flCurrentHeadYaw;
	float m_flCurrentHeadPitch;
	*/

	// Spy cigarette smoke
	bool m_bCigaretteSmokeActive;

	// Medic callout particle effect
	CNewParticleEffect	*m_pSaveMeEffect;
	// Player typing particle effect
	CNewParticleEffect	*m_pChattingEffect;

	bool m_bUpdateObjectHudState;

public:

	CTFPlayerShared m_Shared;

// Called by shared code.
public:

	void DoAnimationEvent( PlayerAnimEvent_t event, int nData = 0 );

	CTFPlayerAnimState *m_PlayerAnimState;

	QAngle	m_angEyeAngles;
	CInterpolatedVar< QAngle >	m_iv_angEyeAngles;

	CNetworkHandle( C_TFItem, m_hItem );

	CNetworkHandle( C_TFWeaponBase, m_hOffHandWeapon );

	int				m_iOldPlayerClass;	// Used to detect player class changes
	bool			m_bIsDisplayingNemesisIcon;

	int				m_iSpawnCounter;

	bool			m_bSaveMeParity;
	bool			m_bOldSaveMeParity;
	bool			m_bChatting;

	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	bool			m_bWaterExitEffectActive;

	CMaterialReference	m_InvulnerableMaterial;


	// Burning
	CSoundPatch			*m_pBurningSound;
	CNewParticleEffect	*m_pBurningEffect;
	CNewParticleEffect	*m_pLightningParticle_tp;
	float				m_flBurnEffectStartTime;
	float				m_flBurnEffectEndTime;

	CNewParticleEffect	*m_pDisguisingEffect;
	float m_flDisguiseEffectStartTime;
	float m_flDisguiseEndEffectStartTime;

	EHANDLE					m_hFirstGib;
	CUtlVector<EHANDLE>		m_hSpawnedGibs;

	int				m_iOldTeam;
	int				m_iOldClass;
	int				m_iOldDisguiseTeam;
	int				m_iOldDisguiseClass;

	bool			m_bDisguised;
	int				m_iPreviousMetal;

	int GetNumActivePipebombs( void );

	//int				m_iSpyMaskBodygroup;

	bool			m_bUpdatePlayerAttachments;
	CHandle<C_PlayerAttachedModel>	m_hPartyHat;
	CHandle<C_PlayerAttachedModel>	m_hSpyMask;
	CHandle<C_PlayerAttachedModel>	m_hShieldEffect;
	typedef CHandle<C_PlayerAttachedModel>	CosmeticHandle;
	CUtlVector<CosmeticHandle>		m_hCosmetic;
	
	virtual void CalcVehicleView(IClientVehicle* pVehicle, Vector& eyeOrigin, QAngle& eyeAngles, float& zNear, float& zFar, float& fov);
	virtual void CalcPlayerView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
	virtual void CalcViewRoll(QAngle& eyeAngles);
	virtual void CalcViewBob(Vector& eyeOrigin);
	virtual void CalcViewIdle(QAngle& eyeAngles);

	float ViewBob;
	double BobTime;
	float BobLastTime;
	float IdleScale;

	// Ladder related data
	EHANDLE			m_hLadder;
	LadderMove_t	m_LadderMove;
	
	Color TennisBall;
private:

	float m_flWaterImpactTime;

	// Gibs.
	CUtlVector<breakmodel_t>	m_aGibs;

	C_TFPlayer( const C_TFPlayer & );

	int		m_iAccount;
public:
	// Get how much $$$ this guy has.
	int GetAccount() const;
	
	bool				IsRetroModeOn() { return m_bRetroMode; }
	CNetworkVar( bool, m_bRetroMode );
};

class C_TFRagdoll : public C_BaseFlex
{
public:

	DECLARE_CLASS( C_TFRagdoll, C_BaseFlex );
	DECLARE_CLIENTCLASS();
	
	C_TFRagdoll();
	~C_TFRagdoll();

	virtual void OnDataChanged( DataUpdateType_t type );

	IRagdoll* GetIRagdoll() const;

	void ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName );

	void ClientThink( void );
	void StartFadeOut( float fDelay );
	void EndFadeOut();

	EHANDLE GetPlayerHandle( void ) 	
	{
		if ( m_iPlayerIndex == TF_PLAYER_INDEX_NONE )
			return NULL;
		return cl_entitylist->GetNetworkableHandle( m_iPlayerIndex );
	}

	bool IsRagdollVisible();
	bool  m_bGib;
	bool  IsGib() { return m_bGib; }
	float GetBurnStartTime() { return m_flBurnEffectStartTime; }

	virtual void SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights );

	// c_baseanimating functions
	virtual void BuildTransformations( CStudioHdr *pStudioHdr, Vector *pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList &boneComputed );

	// GORE
	void ScaleGoreBones( void );
	void InitDismember( void );

	void DismemberHead( );
	void DismemberLeftArm( bool bLevel );
	void DismemberRightArm( bool bLevel );
	void DismemberLeftLeg( bool bLevel );
	void DismemberRightLeg( bool bLevel );

	virtual float FrameAdvance( float flInterval = 0.0f );

	virtual C_BaseEntity *GetItemTintColorOwner( void )
	{
		EHANDLE hPlayer = GetPlayerHandle();
		return hPlayer.Get();
	}

	int m_HeadBodygroup;
	int	m_LeftArmBodygroup;
	int	m_RightArmBodygroup;
	int	m_LeftLegBodygroup;
	int	m_RightLegBodygroup;
	
private:
	
	C_TFRagdoll( const C_TFRagdoll & ) {}
	void Interp_Copy( C_BaseAnimatingOverlay *pSourceEntity );

	void CreateTFRagdoll( void );
	void CreateTFGibs( void );
private:

	CNetworkVector( m_vecRagdollVelocity );
	CNetworkVector( m_vecRagdollOrigin );
	int	  m_iPlayerIndex;
	float m_fDeathTime;
	bool  m_bFadingOut;
	bool  m_bBurning;
	int	  m_iTeam;
	int	  m_iClass;
	float m_flBurnEffectStartTime;	// start time of burning, or 0 if not burning
	float m_flDeathAnimationTime; // start time of burning, or 0 if not burning

	// gore stuff
	CNetworkVar( unsigned short, m_iGoreHead );
	CNetworkVar( unsigned short, m_iGoreLeftArm );
	CNetworkVar( unsigned short, m_iGoreRightArm );
	CNetworkVar( unsigned short, m_iGoreLeftLeg );
	CNetworkVar( unsigned short, m_iGoreRightLeg );

	// takedamageinfo.h
	//int				m_bitsDamageType;
	int				m_iDamageCustom;
};

inline C_TFPlayer* ToTFPlayer( C_BaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	Assert( dynamic_cast<C_TFPlayer*>( pEntity ) != 0 );
	return static_cast< C_TFPlayer* >( pEntity );
}

#endif // C_TF_PLAYER_H
