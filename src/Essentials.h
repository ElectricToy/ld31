//
//  Essentials.h
//  ld31
//
//  Created by Jeff Wofford on 2013/11/19.
//  Copyright (c) 2013 Jeff Wofford. All rights reserved.
//

#ifndef ld31_Essentials_h
#define ld31_Essentials_h

#include "FreshVector.h"
#include "Color.h"
#include "Object.h"
#include "PropertyTraits.h"
#include "ObjectPtr.h"
#include "CommandProcessor.h"
#include "MovieClip.h"
#include "ClassFilter.h"
#include "FreshFile.h"
#include "DisplayPackage.h"
#include "AudioSystem.h"
#include "StructSerialization.h"

#if FRESH_TELEMETRY_ENABLED
#	include "UserTelemetry.h"
#endif

namespace fr
{
	class Sound;
}

namespace ld
{
	using fr::ClassInfo;
	using fr::ClassName;
	using fr::path;
	using fr::XmlElement;
	using fr::real;
	using fr::uint;
	using fr::vec2;
	using fr::vec3;
	using fr::vec4;
	typedef fr::Vector2i vec2i;
	typedef fr::Vector2ui vec2ui;
	using fr::mat4;
	using fr::Color;
	using fr::SmartPtr;
	using fr::WeakPtr;
	using fr::TimeType;
	using fr::Range;
	using fr::rect;
	using fr::Rectanglei;
	using fr::angle;
	using fr::clamp;
	using fr::lerp;
	using fr::ObjectId;
	using fr::ClassFilter;
	using fr::dynamic_freshptr_cast;
	
	const real GROUND_Y = 100;
	const real DEEP_GROUND_Y = 110;
	
	const real WORLD_PER_TILE = 16;
	const real HALF_WORLD_PER_TILE = WORLD_PER_TILE * 0.5f;
	
	const rect LEGAL_BOUNDS( 2 * 16, 2 * 16, 55*16, 40*16 );		// Legal for the player to be in.
	const vec2 WORLD_CENTER( LEGAL_BOUNDS.midpoint() );
	
	const rect HOME_INNER_BOUNDS( vec2( 25, 18 ) * 16, vec2( 33, 24 ) * 16 );
	
	inline vec2 snapToGrid( const vec2& pos )
	{
		return vec2{
			fr::roundToNearest( pos.x - HALF_WORLD_PER_TILE, WORLD_PER_TILE ) + HALF_WORLD_PER_TILE,
			fr::roundToNearest( pos.y - HALF_WORLD_PER_TILE, WORLD_PER_TILE ) + HALF_WORLD_PER_TILE};
	}
	
	typedef std::pair< ClassInfo::cptr, size_t > ClassWeight;
	typedef std::vector< ClassWeight > ClassWeights;

	inline ClassInfo::cptr randomClass( const ClassWeights& classWeights )
	{
		std::vector< ClassInfo::cptr > expanded;
		
		for( const auto& classWeight : classWeights )
		{
			for( size_t i = 0; i < classWeight.second; ++i )
			{
				expanded.push_back( classWeight.first );
			}
		}
		
		if( !expanded.empty() )
		{
			return fr::randomElement( expanded );
		}
		else
		{
			return nullptr;
		}
	}
	
}

#endif
