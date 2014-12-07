//
//  ldWorld.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldWorld.h"
#include "AppStage.h"
#include "Camera.h"
#include "Human.h"
#include "Item.h"
#include "ldTile.h"
#include "Monster.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldWorld )
	DEFINE_VAR( ldWorld, ClassWeights, m_monsterClassWeights );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldWorld )
	
	ldTile& ldWorld::tileAt( const vec2& pos ) const
	{
		return static_cast< ldTile& >( tileGrid().getTile( pos ));
	}
	
	SmartPtr< Item > ldWorld::itemInTile( const vec2& pos ) const
	{
		return tileAt( pos ).containedItem();
	}
	
	fr::Vector2i ldWorld::nearestTile( const vec2& pos, std::function< real( const ldTile&, const Vector2i& ) >&& filter ) const
	{
		const auto& myTileGrid = tileGrid();
		
		const auto& dims = myTileGrid.extents();
		
		real nearestDistSquared = Infinity;
		fr::Vector2i nearest;
		
		for( fr::Vector2i tilePos( 0, 0 ); tilePos.y < dims.y; ++tilePos.y )
		{
			for( tilePos.x = 0; tilePos.x < dims.x; ++tilePos.x )
			{
				const auto& tile = static_cast< const ldTile& >( myTileGrid.getTile( tilePos ));
				
				const real distanceScalar = filter( tile, tilePos );
				
				if( distanceScalar > 0 )
				{
					const real distSquared = distanceScalar * distanceSquared( vector_cast< real >( tilePos ), pos );
					
					if( distSquared < nearestDistSquared )
					{
						nearest = tilePos;
						nearestDistSquared = distSquared;
					}
				}
			}
		}
		
		return nearest;
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
		maybeSpawnMonsters();
		
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
	
	void ldWorld::maybeSpawnMonsters()
	{
		// TODO Waves and so forth.
		
		if( pctChance( stage().secondsPerFrame() * 8 ))
		{
			auto monsterClass = randomClass( m_monsterClassWeights );
			if( monsterClass )
			{
				dev_trace( "Spawning monster of class " << monsterClass->className() );
				
				const auto& myTileGrid = tileGrid();
				
				const size_t afterTileGrid = getChildIndex( &myTileGrid ) + 1;
				
				const auto& dims = myTileGrid.extents();
				
				std::vector< Vector2i > potentialSpawnPoints;
				
				for( Vector2i pos( 0, 0 ); pos.y < dims.y; ++pos.y )
				{
					for( pos.x = 0; pos.x < dims.x; ++pos.x )
					{
						const auto& tile = static_cast< const ldTile& >( myTileGrid.getTile( pos ));
						if( tile.isMonsterSpawner() )
						{
							potentialSpawnPoints.push_back( pos );
						}
					}
				}
			
				if( !potentialSpawnPoints.empty() )
				{
					auto spawnTile = randomElement( potentialSpawnPoints );
					
					auto monster = createObject< Monster >( *monsterClass );
					monster->position( myTileGrid.tileCenter( spawnTile ));
					addChildAt( monster, afterTileGrid );
				}
			}
		}
	}
}

