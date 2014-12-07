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
	
	class ldWorld : public fr::World
	{
		FRESH_DECLARE_CLASS( ldWorld, World );
	public:
		
		fr::TileGrid& tileGrid() const;
		
		SmartPtr< Human > player();

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
		
		SmartPtr< Item > itemInTile( const vec2& pos ) const;
		
		virtual void onBeginPlay() override;
		
	protected:
		
		void updateActorCollisions();
		void checkCollision( ldActor& a, ldActor& b );
		virtual void maybeSpawnMonsters();

	private:
		
		VAR( ClassWeights, m_monsterClassWeights );
	};
	
}

#endif
