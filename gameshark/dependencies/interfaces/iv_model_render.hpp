#pragma once
#include "../../source-sdk/structs/materials.hpp"

struct draw_model_state_t {
public:
	studio_hdr_t		*studio_hdr;
	studiohwdata_t		*studio_hw_data;
	i_client_renderable	*renderable;
	const matrix3x4_t	*model_to_world;
	studio_decal_handle_t	decals;
	int			drawflags;
	int			lod;
};

class iv_model_render {
	public:
	void override_material( i_material *material ) {
		using fn = void( __thiscall * )( iv_model_render *, i_material *, int, int );
		return ( *( fn ** ) this )[ 1 ]( this, material, 0, 0 );
	}
};

#pragma region studiorender_enumerations
enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,
	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,
	STUDIORENDER_DRAW_ACCURATETIME = 0x10,
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,
	STUDIORENDER_DRAW_WIREFRAME = 0x80,
	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,
	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,
	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,
	STUDIORENDER_GENERATE_STATS = 0x8000,
};

enum EDrawModelFlags : unsigned int
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	STUDIO_SKIP_DECALS = 0x10000000,
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	STUDIO_TRANSPARENCY = 0x80000000
};

enum EOverrideType : int
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE
};
#pragma endregion

struct MaterialLightingState_t
{
	vec3_t			vecAmbientCube[6];
	vec3_t			vecLightingOrigin;
	int				nLocalLightCount;
};

struct DrawModelResults_t;
struct ColorMeshInfo_t;
struct StudioDecalHandle_t { int iUnused; };
struct DrawModelInfo_t
{
	studio_hdr_t* pStudioHdr;
	studiohwdata_t* pHardwareData;
	StudioDecalHandle_t		hDecals;
	int						iSkin;
	int						iBody;
	int						iHitboxSet;
	void* pClientEntity;
	int						iLOD;
	ColorMeshInfo_t* pColorMeshes;
	bool					bStaticLighting;
	MaterialLightingState_t	LightingState;
};

class i_studio_render
{
public:
	void set_color_modulation(float const* arrColor)
	{
		using fn = void(__thiscall*)(void*, float const*);
		(*(fn**)this)[27](this, arrColor);
	}
	void set_color_modulation(Color arrColor)
	{
		using fn = void(__thiscall*)(void*, float, float, float);
		(*(fn**)this)[27](this, arrColor.r / 255.0f, arrColor.g / 255.0f, arrColor.b / 255.0f);
	}

	void set_alpha_modulation(float flAlpha)
	{
		using fn = void(__thiscall*)(void*, float);
		(*(fn**)this)[28](this, flAlpha);
	}

	void forced_material_override(i_material* pMaterial, EOverrideType nOverrideType = OVERRIDE_NORMAL, int nOverrides = 0)
	{
		using fn = void(__thiscall*)(void*, i_material*, EOverrideType, int);
		(*(fn**)this)[33](this, pMaterial, nOverrideType, nOverrides);
	}
};