//
//  ldPlayerController.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldPlayerController.h"
#include "Human.h"
#include "AppStage.h"
#include "HUD.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldPlayerController )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldPlayerController )

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
					myHost->useHeldActor();
					break;
					
				default:
					break;
			}
		}
	}

	FRESH_DEFINE_CALLBACK( ldPlayerController, onStageKeyUp, EventKeyboard )
	{}

}

