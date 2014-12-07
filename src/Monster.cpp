//
//  Monster.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Monster.h"
#include "ldWorld.h"
#include "Human.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Monster )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Monster )
	
	bool Monster::canPickup( const ldActor& other ) const
	{
		return false;		// TODO!!!
//		return alive() && other.isHuman() && Super::canPickup( other ) && other.as< Creature >()->alive();
	}
	
	void Monster::updateAI()
	{
		Super::updateAI();
		
		if( auto player = world().player() )
		{
			TileGrid::Path path;
			tileGrid().findClosestPath( position(), player->position(), path, WORLD_PER_TILE * 0.4f );
			
			TileGrid::WorldSpacePath worldSpacePath;
			worldSpacePath.clear();
			tileGrid().convertToWorldSpacePath( path.begin(), path.end(), std::back_inserter( worldSpacePath ));
			
			if( !worldSpacePath.empty() )
			{
				worldSpacePath.erase( worldSpacePath.begin() );
				
				if( !worldSpacePath.empty() )
				{
					applyControllerImpulse( snapToGrid( worldSpacePath.front() ) - position() );
				}
			}
		}
	}
}

