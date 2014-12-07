//
//  Torch.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Torch.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Torch )
	DEFINE_DVAR( Torch, real, m_maxLightRadius );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Torch )
	
	void Torch::update()
	{
		if( alive() )
		{
			m_lightRadius = lerp( m_lightRadius, m_maxLightRadius, m_lightWakeupLerp );
		}
		
		Super::update();
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
	}
	
}

