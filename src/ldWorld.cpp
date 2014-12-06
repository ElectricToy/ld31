//
//  ldWorld.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldWorld.h"
#include "Camera.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldWorld )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldWorld )
	
	TileGrid& ldWorld::tileGrid() const
	{
		const auto child = getChildByName< TileGrid >( "" );
		ASSERT( child );
		return *child;
	}
	
}

