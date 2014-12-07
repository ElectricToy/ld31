//
//  ldPlayerController.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldPlayerController.h"
#include "Human.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldPlayerController )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldPlayerController )
	
	void ldPlayerController::update()
	{
		if( const auto myHost = host()->as< Human >() )
		{
			vec2 movementImpulse;
			
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
		if( const auto myHost = host()->as< Human >() )
		{
			switch( event.key() )
			{
				case Keyboard::Z:
					myHost->pickupTouchingActor();
					break;
					
				case Keyboard::X:
					myHost->dropHeldActor();
					break;
					
				case Keyboard::C:
					myHost->placeHeldActor();
					break;
					
				default:
					break;
			}
		}
	}

	FRESH_DEFINE_CALLBACK( ldPlayerController, onStageKeyUp, EventKeyboard )
	{}

}

