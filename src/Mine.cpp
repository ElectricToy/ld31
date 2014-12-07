//
//  Mine.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Mine.h"
#include "AppStage.h"
#include "ldWorld.h"
#include "Lighting.h"
using namespace fr;

namespace
{
	const Color PLACID_COLOR = 0xff00b882;
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Mine )
	DEFINE_DVAR( Mine, real, m_dangerRadius );
	DEFINE_DVAR( Mine, real, m_explodeRadius );
	DEFINE_DVAR( Mine, real, m_maxDamage );
	DEFINE_DVAR( Mine, bool, m_harmsHumans );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Mine )
	
	rect Mine::dangerArea() const
	{
		return rect{ position() - m_dangerRadius * 0.5f, position() + m_dangerRadius * 0.5f };
	}
	
	void Mine::update()
	{
		Super::update();
		
		auto desiredRadius = lerp( 2.0f, 16.0f, scaleSinToNormal( std::sin( stage().time() * PI * 0.5f )));
		const auto desiredColor = PLACID_COLOR;
										
		// Update light.
		//
		if( placed() )
		{
			desiredRadius *= 8;

			// Who's near me?
			//
			real nearestHumanDistanceSquared = Infinity;
			real nearestMonsterDistanceSquared = Infinity;
			
			world().touchingActors( dangerArea(), [&]( const ldActor& actor )
								   {
									   if( actor.isMonster() )
									   {
										   nearestMonsterDistanceSquared = std::min( distanceSquared( actor.position(), position() ), nearestMonsterDistanceSquared );
									   }
									   else if( actor.isHuman() )
									   {
										   nearestHumanDistanceSquared = std::min( distanceSquared( actor.position(), position() ), nearestHumanDistanceSquared );
									   }
								   } );
			
			if( nearestMonsterDistanceSquared < m_explodeRadius * m_explodeRadius )
			{
				explode();
			}
			else if( nearestMonsterDistanceSquared < m_dangerRadius * m_dangerRadius )
			{
				m_lightColor = Color::Red;
			}
			else if( nearestHumanDistanceSquared < m_dangerRadius * m_dangerRadius )
			{
				m_lightColor = PLACID_COLOR;
			}
			else
			{
				m_lightColor = Color::Orange;
			}
		}

		m_lightRadius = lerp( m_lightRadius, desiredRadius, 0.06f );
		m_lightColor = colorLerp( m_lightColor, desiredColor, 0.06f );
	}
	
	void Mine::explode()
	{
		// Deal damage.
		//
		world().touchingActors( dangerArea(), [&]( ldActor& actor )
							   {
								   if( actor.alive() && actor.isCreature() )
								   {
									   if( !actor.isHuman() || m_harmsHumans )
									   {
										   const real distSquared = distanceSquared( actor.position(), position() );
										   
										   const real damage = distSquared > 0 ? m_maxDamage / distSquared : m_maxDamage;
										   
										   actor.receiveDamage( damage );
									   }
								   }
							   } );
		
		// Sound effect.
		//
		// TODO
		
		// Flash
		//
		if( lightSource() )
		{
			lightSource()->color( Color::White );
			lightSource()->radius( 480 );
		}
		
		die();
	}
}

