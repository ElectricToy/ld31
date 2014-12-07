//
//  Mine.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Mine.h"
#include "Stage.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Mine )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Mine )
	
	void Mine::update()
	{
		Super::update();
		
		const auto desiredRadius = lerp( 2.0f, 16.0f, scaleSinToNormal( std::sin( stage().time() * PI * 0.5f )));
										
		// Update light.
		//
		if( placed() )
		{
			m_lightRadius = lerp( m_lightRadius, desiredRadius * 8, 0.02f );
		}
		else
		{
			m_lightRadius = lerp( m_lightRadius, desiredRadius, 0.06f );
		}
	}
	
}

