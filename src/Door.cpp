//
//  Door.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Door.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Door )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Door )
	
	void Door::onTouched( ldActor& other )
	{
		Super::onTouched( other );
		
		if( placed() && !isPickedUp() && other.isHuman() )
		{
			gotoAndStop( "open" );
		}
	}
	
	void Door::update()
	{
		Super::update();
		gotoAndStop( "closed" );
	}
}

