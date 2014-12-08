//
//  Missile.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/7/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Missile.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Missile )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Missile )

	void Missile::onTouched( ldActor& other )
	{
		Super::onTouched( other );
		
		if( other.isMonster() )
		{
			other.receiveDamage( 1.0f );
			die();
		}
	}
	
	void Missile::update()
	{
		Super::update();
		
		if( nUpdates() > 40 )
		{
			die();
		}
	}
	
}

