#pragma once
#include "imageformats.h"

enum material_var_flags_t {
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	//material_var_unused = (1 << 9),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17), // obsolete
	material_var_unused = (1 << 18), // unused
	material_var_envmapcameraspace = (1 << 19), // obsolete
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23), // obsolete
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25), // obsolete
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_alpha_modified_by_proxy = (1 << 30),
	material_var_vertexfog = (1 << 31),
};

enum preview_image_retval_t {
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

typedef int ImageFormat;
class IMaterialVar;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;
struct model_t;
class i_material;
class c_studio_hdr;
class c_key_values;
struct studiohwdata_t;
struct color_mesh_info_t;
struct draw_model_info_t;

class i_material_var
{
public:
	void SetFloat(float flValue)
	{
		using original_fn = void(__thiscall*)(void*, float);
		(*(original_fn**)this)[4](this, flValue);
	}

	void SetInt(int iValue)
	{
		using original_fn = void(__thiscall*)(void*, int);
		(*(original_fn**)this)[5](this, iValue);
	}

	void SetString(const char* szValue)
	{
		using original_fn = void(__thiscall*)(void*, const char*);
		(*(original_fn**)this)[6](this, szValue);
	}

	void SetVector(float x, float y)
	{
		using original_fn = void(__thiscall*)(void*, float, float);
		(*(original_fn**)this)[10](this, x, y);
	}

	void SetVector(float x, float y, float z)
	{
		using original_fn = void(__thiscall*)(void*, float, float, float);
		(*(original_fn**)this)[11](this, x, y, z);
	}

	void SetVectorComponent(float flValue, int iComponent)
	{
		using original_fn = void(__thiscall*)(void*, float, int);
		(*(original_fn**)this)[26](this, flValue, iComponent);
	}
};

class IHandleEntity
{
public:
	virtual							~IHandleEntity() { }
	virtual void					SetRefEHandle(const unsigned long& hRef) = 0;
	virtual const unsigned long& GetRefEHandle() const = 0;
};

class i_client_unknown : public IHandleEntity
{
public:
	virtual void* GetCollideable() = 0;
	virtual void* GetClientNetworkable() = 0;
	virtual void* GetClientRenderable() = 0;
	virtual void* GetIClientEntity() = 0;
	virtual void* GetBaseEntity() = 0;
	virtual void* GetClientThinkable() = 0;
	virtual void* GetClientAlphaProperty() = 0;
};

class i_client_renderable
{
public:
	virtual i_client_unknown* GetIClientUnknown() = 0;
};
class data_cache_handle_t;
class i_mat_render_context;
struct material_lighting_state_t;
typedef int vertex_format_t;
typedef void* light_cache_handle_t;
typedef void* studio_decal_handle_t;
typedef int material_property_types_t;
typedef unsigned short model_instance_handle_t;
using material_handle_t = unsigned short;

struct MaterialSystemConfig_t
{
	void*			    VideoMode;
	float				flMonitorGamma;
	float				flGammaTVRangeMin;
	float				flGammaTVRangeMax;
	float				flGammaTVExponent;
	bool				bGammaTVEnabled;
	bool				bTripleBuffered;
	int					nAASamples;
	int					nForceAnisotropicLevel;
	int					iSkipMipLevels;
	int					nDxSupportLevel;
	unsigned int		uFlags;
	bool				bEditMode;
	unsigned char		dProxiesTestMode;
	bool				bCompressedTextures;
	bool				bFilterLightmaps;
	bool				bFilterTextures;
	bool				bReverseDepth;
	bool				bBufferPrimitives;
	bool				bDrawFlat;
	bool				bMeasureFillRate;
	bool				bVisualizeFillRate;
	bool				bNoTransparency;
	bool				bSoftwareLighting;
	bool				bAllowCheats;
	char				nShowMipLevels;
	bool				bShowLowResImage;
	bool				bShowNormalMap;
	bool				bMipMapTextures;
	unsigned char		uFullbright;
	bool				bFastNoBump;
	bool				bSuppressRendering;
	bool				bDrawGray;
	bool				bShowSpecular;
	bool				bShowDiffuse;
	std::uint32_t		uWindowedSizeLimitWidth;
	std::uint32_t		uWindowedSizeLimitHeight;
	int					nAAQuality;
	bool				bShadowDepthTexture;
	bool				bMotionBlur;
	bool				bSupportFlashlight;
	bool				bPaintEnabled;
	std::byte			pad0[0xC];
};

class i_material {
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual preview_image_retval_t get_preview_image_properties(int* width, int* height, image_format* image_format, bool* is_translucent) const = 0;
	virtual preview_image_retval_t get_preview_image(unsigned char* data, int width, int height, image_format image_format) const = 0;
	virtual int get_mapping_width() = 0;
	virtual int get_mapping_height() = 0;
	virtual int get_num_animation_frames() = 0;
	virtual bool in_material_page(void) = 0;
	virtual void get_material_offset(float* offset) = 0;
	virtual void get_material_scale(float* scale) = 0;
	virtual i_material* get_material_page(void) = 0;
	virtual i_material_var* find_var(const char* var_name, bool* found, bool complain = true) = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	inline void add_ref() { increment_reference_count(); }
	inline void release() { decrement_reference_count(); }
	virtual int get_enumeration_id(void) const = 0;
	virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void recompute_state_snapshots() = 0;
	virtual bool is_translucent() = 0;
	virtual bool is_alpha_tested() = 0;
	virtual bool is_vertex_lit() = 0;
	virtual vertex_format_t get_vertex_format() const = 0;
	virtual bool has_proxy(void) const = 0;
	virtual bool uses_env_cubemap(void) = 0;
	virtual bool needs_tangent_space(void) = 0;
	virtual bool needs_power_of_two_frame_buffer_texture(bool check_specific_to_this_frame = true) = 0;
	virtual bool needs_full_frame_buffer_texture(bool check_specific_to_this_frame = true) = 0;
	virtual bool needs_software_skinning(void) = 0;
	virtual void alpha_modulate(float alpha) = 0;
	virtual void color_modulate(float r, float g, float b) = 0;
	virtual void set_material_var_flag(material_var_flags_t flag, bool on) = 0;
	virtual bool get_material_var_flag(material_var_flags_t flag) const = 0;
	virtual void get_reflectivity(vec3_t& reflect) = 0;
	virtual bool get_property_flag(material_property_types_t  type) = 0;
	virtual bool is_two_sided() = 0;
	virtual void set_shader(const char* shader_name) = 0;
	virtual int get_num_passes(void) = 0;
	virtual int get_texture_memory_bytes(void) = 0;
	virtual void refresh() = 0;
	virtual bool needs_lightmap_blend_alpha(void) = 0;
	virtual bool needs_software_lighting(void) = 0;
	virtual int shader_param_count() const = 0;
	virtual i_material_var** get_shader_params(void) = 0;
	virtual bool is_error_material() const = 0;
	virtual void unused() = 0;
	virtual float get_alpha_modulation() = 0;
	virtual void get_color_modulation(float* r, float* g, float* b) = 0;
	virtual bool is_translucent_under_modulation(float alpha_modulation = 1.0f) const = 0;
	virtual i_material_var* find_var_fast(char const* var_name, unsigned int* token) = 0;
	virtual void set_shader_and_params(c_key_values* key_values) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual bool is_sprite_card() = 0;
	virtual void call_bind_proxy(void* proxy_data) = 0;
	virtual void refresh_preserving_material_vars() = 0;
	virtual bool was_reloaded_from_whitelist() = 0;
	virtual bool set_temp_excluded(bool set, int excluded_dimension_limit) = 0;
	virtual int get_reference_count() const = 0;

	void color_modulate(Color c) {
		using original_fn = void(__thiscall*)(i_material*, float, float, float);
		return (*(original_fn**)this)[28](this, c.r / 255.f, c.g / 255.f, c.b / 255.f);
	}
	void alpha_modulate(Color c) {
		using original_fn = void(__thiscall*)(i_material*, float);
		return (*(original_fn**)this)[27](this, c.a / 255.f);
	}
};
