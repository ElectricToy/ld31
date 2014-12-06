//
//  ldActor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldActor.h"
#include "Stage.h"
#include "ActorController.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldActor )
	DEFINE_VAR( ldActor, vec2, m_stepDirection );
	DEFINE_VAR( ldActor, vec2, m_stepStart );
	DEFINE_DVAR( ldActor, real, m_stepSpeed );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldActor )
	
	void ldActor::update()
	{
		updateStepping();
		
		Super::update();
	}
	
	void ldActor::applyControllerImpulse( const vec2& i )
	{
		auto step = i.normal();
		step.snapToMajorAxis();
		
		if( !isStepping() )
		{
			beginStepping( step );
		}
		else
		{
			m_pendingStepDirection = step;
		}
	}
	
	void ldActor::beginStepping( const vec2& dir )
	{
		ASSERT( !isStepping() );
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		m_stepStart = snapToGrid( position() );
		m_stepDirection = dir.normal();
	}

	void ldActor::stopStepping()
	{
		m_stepDirection.setToZero();
		position( snapToGrid( position() ));
	}
	
	bool ldActor::isStepping() const
	{
		return !m_stepDirection.isZero();
	}
	
	void ldActor::updateStepping()
	{
		if( isStepping() )
		{
			// Move.
			//
			const auto newPos = position() + m_stepDirection * WORLD_PER_TILE * m_stepSpeed * stage().secondsPerFrame();
			position( newPos );
			
			// Done yet?
			//
			const auto distTraveled = ( newPos - m_stepStart ).dot( m_stepDirection );
			
			if( distTraveled >= WORLD_PER_TILE )
			{
				// Done stepping.
				//
				stopStepping();
			}
		}
		
		if( !isStepping() && !m_pendingStepDirection.isZero() )
		{
			beginStepping( m_pendingStepDirection );
			m_pendingStepDirection.setToZero();
		}
	}
	
	void ldActor::onLanded( const vec2& hitNormal )
	{
		Super::onLanded( hitNormal );
		stopStepping();
	}
	
	void ldActor::onBumpedWall( const vec2& hitNormal )
	{
		stopStepping();
	}
}

