//
//  ldWorld.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldWorld.h"
#include "Camera.h"
#include "Human.h"
#include "Item.h"
#include "ldTile.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldWorld )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldWorld )
	
	SmartPtr< Item > ldWorld::itemInTile( const vec2& pos ) const
	{
		const auto& tile = static_cast< const ldTile& >( tileGrid().getTile( pos ));
		return tile.containedItem();
	}
	
	void ldWorld::onBeginPlay()
	{
		Super::onBeginPlay();
		
		// Visit all tiles in the tilegrid that area capable of holding treasures,
		// and spawn them.
		//
		const auto& myTileGrid = tileGrid();
		
		const size_t afterTileGrid = getChildIndex( &myTileGrid ) + 1;
		
		const auto& dims = myTileGrid.extents();
		
		for( Vector2i pos( 0, 0 ); pos.y < dims.y; ++pos.y )
		{
			for( pos.x = 0; pos.x < dims.x; ++pos.x )
			{
				const auto& tile = static_cast< const ldTile& >( myTileGrid.getTile( pos ));
				auto item = tile.createRandomItem();
				
				if( item )
				{
					item->position( myTileGrid.tileCenter( pos ));
					addChildAt( item, afterTileGrid );
				}
			}
		}
	}
	
	TileGrid& ldWorld::tileGrid() const
	{
		const auto child = getChildByName< TileGrid >( "" );
		ASSERT( child );
		return *child;
	}
	
	SmartPtr< Human > ldWorld::player()
	{
		Human::ptr thePlayer;
		forEachChild< Human >( [&]( Human& human )
							  {
								  if( human.isPlayer() )
								  {
									  thePlayer = &human;
								  }
							  } );
		
		return thePlayer;
	}

	void ldWorld::update()
	{
		Super::update();
		
		updateActorCollisions();
	}
	
	void ldWorld::updateActorCollisions()
	{
		// Build list of actors.
		//
		std::vector< ldActor::ptr > actors;
		forEachChild< ldActor >( [&]( ldActor& actor )
								 {
									 if( actor.mayCollide() )
									 {
										 actors.push_back( &actor );
									 }
								 } );
		
		for( size_t i = 0; i < actors.size(); ++i )
		{
			const auto outer = actors[ i ];
			ASSERT( outer );
			
			for( size_t j = i + 1; j < actors.size(); ++j )
			{
				const auto inner = actors[ j ];
				ASSERT( inner );				
				ASSERT( inner != outer );
				
				checkCollision( *outer, *inner );
			}
		}
	}
	
	void ldWorld::checkCollision( ldActor& a, ldActor& b )
	{
		if( a.collisionBounds().doesOverlap( b.collisionBounds() ))
		{
			a.onTouched( b );
			b.onTouched( a );
		}
	}
}

