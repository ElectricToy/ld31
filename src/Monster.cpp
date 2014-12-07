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

namespace
{
	const real MIN_DIST_TO_TOUCH_NODE = 2.0f;
	const real MIN_DIST_TO_TOUCH_NODE_SQUARED = MIN_DIST_TO_TOUCH_NODE * MIN_DIST_TO_TOUCH_NODE;
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Monster )
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Monster )
	
	bool Monster::canPickup( const ldActor& other ) const
	{
		return false;		// TODO!!!
//		return alive() && other.isHuman() && Super::canPickup( other ) && other.as< Creature >()->alive();
	}
	
	void Monster::update()
	{
		if( alive() && !isPickedUp() && !m_worldSpacePath.empty() )
		{
			applyControllerImpulse( snapToGrid( m_worldSpacePath.front() ) - position() );
			
			// Have we reached the next path node?
			//
			if( distanceSquared( position(), m_worldSpacePath.front() ) < MIN_DIST_TO_TOUCH_NODE_SQUARED )
			{
				m_worldSpacePath.erase( m_worldSpacePath.begin() );
			}
		}
		
		Super::update();
	}
	
	void Monster::updateAI()
	{
		Super::updateAI();
		
		if( auto player = world().player() )
		{
			TileGrid::Path path;
			tileGrid().findClosestPath( position(), player->position(), path, WORLD_PER_TILE * 0.4f );
			
			m_worldSpacePath.clear();
			tileGrid().convertToWorldSpacePath( path.begin(), path.end(), std::back_inserter( m_worldSpacePath ));
			
			if( !m_worldSpacePath.empty() )
			{
				m_worldSpacePath.erase( m_worldSpacePath.begin() );
			}
		}
	}
}

