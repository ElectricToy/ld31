//
//  Monster.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Monster.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Monster )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Monster )
	
	bool Monster::canPickup( const ldActor& other ) const
	{
		return alive() && other.isHuman() && Super::canPickup( other ) && other.as< Creature >()->alive();
	}
}

