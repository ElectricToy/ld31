//
//  Monster.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Monster.h"
#include "ldWorld.h"
#include "Human.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Monster )
	DEFINE_DVAR( Monster, real, m_awarenessRadius );
	DEFINE_VAR( Monster, ldActor::wptr, m_pursueee );
	DEFINE_DVAR( Monster, real, m_beginPursuingRadius );
	DEFINE_DVAR( Monster, real, m_giveUpPursuingRadius );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Monster )
	
	bool Monster::canPickup( const ldActor& other ) const
	{
		return false;		// TODO!!!
//		return alive() && other.isHuman() && Super::canPickup( other ) && other.as< Creature >()->alive();
	}
	
	void Monster::update()
	{
		Super::update();
	}
	
	void Monster::updateAI()
	{
		Super::updateAI();
		
		vec2 destination;
		
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
									   
									   // Worth pursuing?
									   //
									   if( distSquared < m_beginPursuingRadius * m_beginPursuingRadius )
									   {
										   // Already pursuing this one?
										   //
										   if( m_pursueee == &actor )
										   {
											   distSquared *= 0.75f;		// Shorter. Histeresis.
										   }
										   
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
		
		if( !destination.isZero() )
		{
			travelTo( destination );
		}
		else
		{
			stopTravel();
		}
	}
}

