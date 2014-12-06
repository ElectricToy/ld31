//
//  Body.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/5/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Body.h"
#include "Stage.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Body )
	
	DEFINE_DVAR( Body, real, m_mass );
	DEFINE_DVAR( Body, real, m_airDrag );
	DEFINE_DVAR( Body, real, m_stiffness );
	DEFINE_DVAR( Body, real, m_windDrag );
	DEFINE_VAR( Body, vec2, m_lastPosition );
	DEFINE_VAR( Body, vec2, m_acceleration );
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Body )

	void Body::applyGravity( const vec2& g )
	{
		if( m_mass > 0 )
		{
			m_acceleration += g;
		}
	}
	
	void Body::applyWind( const vec2& f )
	{
		if( m_mass > 0 )
		{
			m_acceleration += m_windDrag * f / m_mass;
		}
	}

	void Body::applyImpulse( const vec2& i )
	{
		if( m_mass > 0 )
		{
			m_acceleration += i / m_mass;
		}
	}
	
	void Body::update()
	{
		m_position = updateVerlet( m_position, m_lastPosition, m_airDrag, m_acceleration, static_cast< real >( stage().secondsPerFrame() ));
		
		m_acceleration.setToZero();
		
		m_position.y = std::min( m_position.y, GROUND_Y );
		
		Super::update();
	}
	
	void Body::postLoad()
	{
		Super::postLoad();
		
		m_lastPosition = position();
	}	
}

