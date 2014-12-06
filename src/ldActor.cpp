//
//  ldActor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldActor.h"
#include "ldWorld.h"
#include "Stage.h"
#include "TileGrid.h"
#include "ActorController.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldActor )
	DEFINE_VAR( ldActor, vec2, m_stepDirection );
	DEFINE_VAR( ldActor, vec2, m_stepStart );
	DEFINE_DVAR( ldActor, real, m_stepSpeed );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldActor )

	void ldActor::onAddedToStage()
	{
		Super::onAddedToStage();
		
		position( snapToGrid( position() ));
	}
	
	ldWorld& ldActor::world() const
	{
		const auto ancestor = firstAncestorOfType< ldWorld >( *this );
		ASSERT( ancestor );
		return *ancestor;
	}
	
	TileGrid& ldActor::tileGrid() const
	{
		return world().tileGrid();
	}
	
	void ldActor::update()
	{
		updateStepping();
		
		Super::update();
	}
	
	void ldActor::applyControllerImpulse( const vec2& i )
	{
		if( !isStepping() && !i.isZero() )
		{
			// More than one direction implied here?
			//
			const vec2 steps[ 2 ] =
			{
				vec2( i.x, 0 ),
				vec2( 0, i.y )
			};

			vec2 stepToPursue;
			
			for( int i = 0; i < 2; ++i )
			{
				const auto& potentialStep = steps[ i ];
				if( !potentialStep.isZero() )
				{
					if( canStep( potentialStep ))
					{
						// We could do this. But if it's the direction we're already moving,
						// and we have an alternative, then keep the alternative.
						//
						if( stepToPursue.isZero() || potentialStep != m_lastStepDirection )
						{
							stepToPursue = potentialStep;
						}
					}
				}
			}
			
			if( !stepToPursue.isZero() )
			{
				beginStepping( stepToPursue );
			}
		}
	}

	bool ldActor::canStep( const vec2& dir ) const
	{
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		const auto proposedStart = snapToGrid( position() );
		const auto proposedDest = proposedStart + dir.normal() * WORLD_PER_TILE;
		const fr::Direction fromDirection( -dir );
		
		return tileGrid().getTile( proposedDest ).isNavigable( fromDirection );
	}
	
	void ldActor::beginStepping( const vec2& dir )
	{
		ASSERT( !isStepping() );
		ASSERT( dir.x == 0 || dir.y == 0 );
		
		// Refuse if there's a wall there.
		//
		if( canStep( dir ))
		{
			m_stepStart = snapToGrid( position() );
			m_stepDirection = dir.normal();
		}
	}

	void ldActor::stopStepping()
	{
		m_lastStepDirection = m_stepDirection;
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

