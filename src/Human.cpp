//
//  Human.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Human.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Human )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Human )
	
	bool Human::canPickup( const ldActor& other ) const
	{
		return isPlayer() && other.isItem() && Super::canPickup( other );
	}
	
	bool Human::isPlayer() const
	{
		return controller() != nullptr;
	}
	
}

