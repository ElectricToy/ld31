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
	DEFINE_DVAR( Body, real, m_buriedDrag );
	DEFINE_DVAR( Body, real, m_stiffness );
	DEFINE_DVAR( Body, real, m_windDrag );
	DEFINE_VAR( Body, vec2, m_lastPosition );
	DEFINE_VAR( Body, vec2, m_acceleration );
	DEFINE_VAR( Body, angle, m_lastRotation );
	DEFINE_VAR( Body, angle, m_torque );
	
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
	
	void Body::applyTorque( const angle t )
	{
		if( m_mass > 0 )
		{
			m_torque += t / m_mass;
		}
	}
	
	void Body::update()
	{
		const auto priorPosition = m_lastPosition;
		
		const auto drag = underground() ? m_buriedDrag : m_airDrag;
		
		m_position = updateVerlet( m_position, m_lastPosition, drag, m_acceleration, static_cast< real >( stage().secondsPerFrame() ));
		
		m_acceleration.setToZero();
		
		m_rotation = updateVerlet( m_rotation, m_lastRotation, drag, m_torque, static_cast< real >( stage().secondsPerFrame() ));
		
		if( underground() )
		{
			// Pull toward last position.
			//
			m_position = lerp( m_position, priorPosition, undergroundness() );
		}
		
		Super::update();
	}
	
	void Body::postLoad()
	{
		Super::postLoad();
		
		m_lastPosition = position();
	}	
}

