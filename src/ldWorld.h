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
#include "TimeServer.h"

namespace ld
{
	class Human;
	class Item;
	class ldTile;
	
	class ldWorld : public fr::World, public fr::TimeServer
	{
		FRESH_DECLARE_CLASS( ldWorld, World );
	public:
		
		// From TimeServer
		virtual TimeType time() const override;
		
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
		
		template< typename ActorT, typename FunctionT >
		size_t countActors( FunctionT&& fn ) const
		{
			size_t count = 0;
			forEachChild< ActorT >( [&]( const ldActor& actor )
								   {
									   if( fn( actor ))
									   {
										   ++count;
									   }
								   } );
			return count;
		}

		template< typename ActorT >
		size_t countActors() const
		{
			size_t count = 0;
			forEachChild< ActorT >( [&]( const ldActor& actor )
								   {
									   ++count;
								   } );
			return count;
		}
		
		vec2 findOpenItemSpawnPosition() const;
		
		size_t currentSpawnPhase() const;

	protected:
		
		void updateActorCollisions();
		void checkCollision( ldActor& a, ldActor& b );
		virtual void maybeSpawnMonsters();
		
		void spawnMonster();
		
		void pickActiveHuman();
		
		void provideEssentials( size_t minMines, size_t minTorches );

	private:
		
		VAR( ClassInfo::cptr, m_playerControllerClass );
		DVAR( int, m_lastActiveUpdate, 0 );
		DVAR( bool, m_playerHasMoved, false );
		DVAR( size_t, m_minInitialTorches, 3 );
		DVAR( size_t, m_minInitialMines, 3 );
		DVAR( int, m_nextSpawnTime, -1 );
		
		size_t m_lastSpawnPhase = -1;
		
		FRESH_DECLARE_CALLBACK( ldWorld, onTimeToProvide, fr::Event )
		FRESH_DECLARE_CALLBACK( ldWorld, onTimeForPreparationWarning, fr::Event )
		
	};
	
}

#endif
