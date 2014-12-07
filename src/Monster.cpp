//
//  Monster.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Monster.h"
#include "ldWorld.h"
#include "ldTile.h"
#include "Human.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Monster )
	DEFINE_DVAR( Monster, real, m_awarenessRadius );
	DEFINE_VAR( Monster, ldActor::wptr, m_pursueee );
	DEFINE_DVAR( Monster, real, m_beginPursuingRadius );
	DEFINE_DVAR( Monster, real, m_giveUpPursuingRadius );
	DEFINE_VAR( Monster, fr::Vector2i, m_exitDestination );
	DEFINE_VAR( Monster, ClassWeights, m_dropItemWeights );
	DEFINE_DVAR( Monster, size_t, m_maxItemsToDrop );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Monster )
	
	bool Monster::canPickup( const ldActor& other ) const
	{
		return other.isHuman() && !other.isPlayer() && Super::canPickup( other );
	}
	
	void Monster::update()
	{
		Super::update();
		
		// Reached a spawn tile while holding something?
		//
		if( heldActor() && world().tileAt( position() ).isMonsterSpawner() )
		{
			heldActor()->die();
			heldActor()->markForDeletion();
			die();
			markForDeletion();
		}
	}
	
	void Monster::updateAI()
	{
		Super::updateAI();
		
		vec2 destination;
		
		if( heldActor() )
		{
			m_pursueee = nullptr;
			
			// Head back to the nearest exit.
			//
			auto tilePos = world().nearestTile( position(), [&]( const ldTile& tile, const Vector2i& pos )
								{
									if( tile.isMonsterSpawner() )
									{
										return pos == m_exitDestination ? 0.25f : 1.0f;
									}
									else
									{
										return 0.0f;
									}
								} );
			
			ASSERT( !tilePos.isZero() );
			
			if( m_exitDestination != tilePos )
			{
				m_exitDestination = tilePos;
				destination = tileGrid().tileCenter( m_exitDestination );
			}
			// Else no change in destination, therefore no new travel command.
		}
		else
		{
			m_exitDestination.setToZero();
			
			// Look in my awareness area.
			//
			real nearestHumanDistanceSquared = Infinity;
			ldActor::ptr nearestHuman;
			
			world().touchingActors( rect{ position() - m_awarenessRadius * 0.5f, position() + m_awarenessRadius * 0.5f },
								   [&]( ldActor& actor )
								   {
									   if( actor.isHuman() )
									   {
										   real distSquared = distanceSquared( actor.position(), position() );
										   
										   // Already pursuing this one?
										   //
										   if( m_pursueee == &actor )
										   {
											   distSquared *= 0.5f;		// Shorter. Histeresis.
										   }
										   
										   // Worth pursuing?
										   //
										   if( distSquared < m_beginPursuingRadius * m_beginPursuingRadius )
										   {
											   if( distSquared < nearestHumanDistanceSquared )
											   {
												   nearestHumanDistanceSquared = distSquared;
												   nearestHuman = &actor;
											   }
										   }
									   }
								   } );
			
			if( nearestHuman )
			{
				m_pursueee = nearestHuman;
			}
			else
			{
				// Has the pursuee gotten too far away?
				//
				if( m_pursueee && distanceSquared( m_pursueee->position(), position() ) > m_giveUpPursuingRadius )
				{
					m_pursueee = nullptr;
				}
			}
			
			if( m_pursueee )
			{
				destination = m_pursueee->position();
			}
			else
			{
				destination = WORLD_CENTER;
			}
		}
		
		if( !destination.isZero() )
		{
			travelTo( destination );
		}
	}
	
	void Monster::die()
	{
		// Drop treasure.
		//
		const size_t nItemsToDrop = randInRange( 1UL, m_maxItemsToDrop );
		
		for( size_t i = 0; i < nItemsToDrop; ++i )
		{
			if( auto itemClass = randomClass( m_dropItemWeights ))
			{
				world().createItemNear( *itemClass, position() );
			}
		}
		
		Super::die();
	}
}

