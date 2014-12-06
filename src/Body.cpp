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
	DEFINE_DVAR( Body, real, m_damping );
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
	
	void Body::applyImpulse( const vec2& i )
	{
		if( m_mass > 0 )
		{
			m_acceleration += i / m_mass;
		}
	}
	
	void Body::update()
	{
		updateVerlet( m_position, m_lastPosition, m_damping, m_acceleration, static_cast< real >( stage().secondsPerFrame() ));
		
		m_acceleration.setToZero();
		
		Super::update();
	}
	
	void Body::postLoad()
	{
		Super::postLoad();
		
		m_lastPosition = position();
	}
	
	
}

