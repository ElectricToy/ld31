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
#include "ldActor.h"

namespace ld
{
	class Human;
	class Item;
	class ldTile;
	
	class ldWorld : public fr::World
	{
		FRESH_DECLARE_CLASS( ldWorld, World );
	public:
		
		fr::TileGrid& tileGrid() const;
		
		SmartPtr< Human > player();
		SmartPtr< const Human > player() const;

		bool isGameActive() const;				// I.e. some humans still alive.
		
		int timePlayedSeconds() const;
		
		size_t numFreeHumans() const;			// Humans that are alive and not captured.
		size_t numLivingHumans() const;			// Humans that are alive but possibly captured.
		size_t numHumans() const;				// Humans that are in any state.
		
		virtual void update() override;
		
		template< typename FunctionT >
		void touchingActors( const rect& bounds, FunctionT&& fn )
		{
			forEachChild< ldActor >( [&]( ldActor& actor )
								 {
									 if( actor.mayCollide() && actor.collisionBounds().doesOverlap( bounds ))
									 {
										 fn( actor );
									 }
								 } );
		}

		ldTile& tileAt( const vec2& pos ) const;
		SmartPtr< Item > itemInTile( const vec2& pos ) const;
		
		fr::Vector2i nearestTile( const vec2& pos, std::function< real( const ldTile&, const fr::Vector2i& ) >&& filter ) const;
		
		SmartPtr< Item > createItemNear( const ClassInfo& itemClass, const vec2& pos );
		
		virtual void onBeginPlay() override;
		
		template< typename FunctionT >
		void dealExplosionDamage( const rect& damageBounds, const vec2& center, real power, FunctionT&& actorFilter )
		{
			touchingActors( damageBounds, [&]( ldActor& actor )
								   {
									   if( actor.alive() && actorFilter( actor ))
									   {
										   const real distSquared = distanceSquared( actor.position(), center );
										   
										   const real damage = distSquared > 0 ? power / distSquared : power;
										   
										   actor.receiveDamage( damage );
									   }
								   } );
		}
		
	protected:
		
		void updateActorCollisions();
		void checkCollision( ldActor& a, ldActor& b );
		virtual void maybeSpawnMonsters();
		
		void pickActiveHuman();

	private:
		
		VAR( ClassWeights, m_monsterClassWeights );
		VAR( ClassInfo::cptr, m_playerControllerClass );
		DVAR( int, m_lastActiveUpdate, 0 );
	};
	
}

#endif
