#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_manager_t {
public:
	struct GlowObject_t
	{
		void Set(Color color, int nRenderStyle = 0)
		{
			this->vecColor = vec3_t(color.r / 255.f, color.g / 255.f, color.b / 255.f);
			this->flAlpha = color.a / 255.f;
			this->flBloomAmount = 1.0f;
			this->bRenderWhenOccluded = true;
			this->bRenderWhenUnoccluded = false;
			this->nRenderStyle = nRenderStyle;
		}

		inline bool IsEmpty() const
		{
			return nNextFreeSlot != -2;
		}

		c_base_entity*	pEntity;
		vec3_t			vecColor;
		float			flAlpha;
		std::byte		pad0[0x8];
		float			flBloomAmount;
		float			flLocalPlayerIsZeroPoint;
		bool			bRenderWhenOccluded;
		bool			bRenderWhenUnoccluded;
		bool			bFullBloomRender;
		std::byte		pad1[0x1];
		int				iFullBloomStencilTestValue;
		int				nRenderStyle;
		int				nSplitScreenSlot;
		int				nNextFreeSlot;
	};

	utl_vector<GlowObject_t> vecGlowObjectDefinitions;
	int nFirstFreeSlot;
};