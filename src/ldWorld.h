//
//  ldWorld.h
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#ifndef FreshApp_ldWorld_h
#define FreshApp_ldWorld_h

#include "Essentials.h"
#include "World.h"
#include "TileGrid.h"

namespace ld
{
	class ldWorld : public fr::World
	{
		FRESH_DECLARE_CLASS( ldWorld, World );
	public:
		
		fr::TileGrid& tileGrid() const;
		
	private:
		
	};
	
}

#endif
