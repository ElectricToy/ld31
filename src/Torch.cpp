//
//  Torch.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Torch.h"
#include "AppStage.h"
using namespace fr;

namespace
{
	const TimeType FIZZLE_TIME = 30.0;
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Torch )
	DEFINE_DVAR( Torch, real, m_maxLightRadius );
	DEFINE_DVAR( Torch, TimeType, m_deathTime );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Torch )
	
	void Torch::update()
	{
		if( alive() )
		{
			real desiredLightRadius = m_maxLightRadius;
			
			// Update lifespan/fizzle.
			//
			const auto lifeRemaining = std::max( 0.0, m_deathTime - stage().time() );
			if( lifeRemaining <= FIZZLE_TIME )
			{
				desiredLightRadius = lerp( 2.0f, m_maxLightRadius, static_cast< real >( std::pow( lifeRemaining / FIZZLE_TIME, 0.5 )));
				
				if( lifeRemaining <= 0 )
				{
					die();
				}
			}
			
			m_lightRadius = lerp( m_lightRadius, desiredLightRadius, m_lightWakeupLerp );
		}
		
		if( doUpdate() )
		{
			Super::update();
		}
	}
	
	void Torch::onAddedToStage()
	{
		Super::onAddedToStage();
		
		// Randomize color.
		//
		real hue = randInRange( 0.0f, 360.0f );
		
		m_lightColor.fromHSVA( vec4( hue, 1.0f, 1.0f, 1.0f ));
		
		// Randomize size.
		//
		m_maxLightRadius = randInRange( 100.0f, 180.0f );
		
		m_lightRadius = 0;
		
		m_lightWakeupLerp = randInRange( 0.01f, 0.1f );
		
		if( auto flame = getDescendantByName< DisplayObject >( "_flame" ))
		{
			flame->color( m_lightColor );
		}
		
		// Randomize lifespan.
		//
		m_deathTime = stage().time() + 60 * randInRange( 5, 15 );
	}

	void Torch::die()
	{
		playSound( "torch_fizzle", position() );
		
		Super::die();
	}
}

