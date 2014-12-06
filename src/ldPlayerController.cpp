//
//  ldPlayerController.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldPlayerController.h"
#include "Actor.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldPlayerController )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldPlayerController )
	
	void ldPlayerController::update()
	{
		if( const auto myHost = host() )
		{
			vec2 movementImpulse;
			
			// TODO make configurable.
			if( Keyboard::isKeyDown( Keyboard::LeftArrow ) || Keyboard::isKeyDown( Keyboard::A ))
			{
				movementImpulse.x -= 1;
			}
			if( Keyboard::isKeyDown( Keyboard::RightArrow ) || Keyboard::isKeyDown( Keyboard::D ))
			{
				movementImpulse.x += 1;
			}
			if( Keyboard::isKeyDown( Keyboard::UpArrow ) || Keyboard::isKeyDown( Keyboard::W ))
			{
				movementImpulse.y -= 1;
			}
			if( Keyboard::isKeyDown( Keyboard::DownArrow ) || Keyboard::isKeyDown( Keyboard::S ))
			{
				movementImpulse.y += 1;
			}
			
			if( !movementImpulse.isZero() )
			{
				myHost->applyControllerImpulse( movementImpulse );
			}
		}
	}
	
	FRESH_DEFINE_CALLBACK( ldPlayerController, onStageKeyDown, EventKeyboard )
	{
		if( host() && event.key() == Keyboard::Space )
		{
			host()->jump( 1.0 );
		}
		
	}
	
}
