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
#include "PlayerController.h"
#include "HUD.h"
#include "Mine.h"
#include "Torch.h"
#include "Missile.h"
#include "Turret.h"
#include "SimpleButton.h"
#include "UIPopup.h"
using namespace fr;

namespace
{
	using namespace ld;
	
	template< typename T >
	void use()
	{
		auto object = createObject< T >();		// TODO
	}

	const int INITIAL_SETUP_TIME_SECONDS = 100;
	const int SECONDS_PER_PHASE = 120;
	
	const std::vector< std::pair< int, int >> PHASE_SPAWN_DELAY_RANGE =
	{
		{ 60, 10 },		// seconds, delta
		{ 45, 10 },
		{ 30, 10 },
		{ 16, 5 },
		{ 8, 2 },
	};
}

namespace ld
{	
	FRESH_DEFINE_CLASS( ldWorld )
	DEFINE_VAR( ldWorld, ClassInfo::cptr, m_playerControllerClass );
	DEFINE_DVAR( ldWorld, int, m_lastActiveUpdate );
	DEFINE_DVAR( ldWorld, bool, m_playerHasMoved );
	DEFINE_DVAR( ldWorld, size_t, m_minInitialTorches );
	DEFINE_DVAR( ldWorld, size_t, m_minInitialTurrets );
	DEFINE_DVAR( ldWorld, size_t, m_minInitialMines );
	DEFINE_DVAR( ldWorld, int, m_nextSpawnTime );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTOR_INERT( ldWorld )
	
	FRESH_CUSTOM_STANDARD_CONSTRUCTOR_NAMING( ldWorld )
	,	TimeServer( createObject< CallbackScheduler >() )
	{}

	TimeType ldWorld::time() const
	{
		return nUpdates() / stage().frameRate();
	}
	
	size_t ldWorld::numFreeHumans() const
	{
		// Humans that are alive and not captured.
		
		size_t count = 0;
		forEachChild< Human >( [&]( const Human& human )
							  {
								  if( human.alive() && !human.isPickedUp() )
								  {
									  ++count;
								  }
							  } );
		return count;
	}
	
	size_t ldWorld::numLivingHumans() const
	{
		// Humans that are alive but possibly captured.
		
		size_t count = 0;
		forEachChild< Human >( [&]( const Human& human )
							  {
								  if( human.alive() )
								  {
									  ++count;
								  }
							  } );
		return count;
	}
	
	size_t ldWorld::numHumans() const
	{
		// Humans that are in any state.
		size_t count = 0;
		forEachChild< Human >( [&]( const Human& human )
							  {
								  ++count;
							  } );
		return count;
	}

	bool ldWorld::isGameActive() const
	{
		return player() != nullptr;
	}
	
	int ldWorld::timePlayedSeconds() const
	{
		return static_cast< int >( m_lastActiveUpdate / stage().frameRate());
	}
	
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
					const real distSquared = distanceScalar * distanceSquared( myTileGrid.tileCenter( tilePos ), pos );
					
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
		// Reference classes (for Emscripten)
		//
		if( name() == "Can't be this name" )		// TODO insane.
		{
			use< Mine >();
			use< Torch >();
			use< Missile >();
			use< Turret >();
			use< SimpleButton >();
			use< UIPopup >();
			use< HUD >();
		}
		
		
		Super::onBeginPlay();
		
		// Visit all tiles in the tilegrid that are capable of holding treasures,
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
					ASSERT( tile.mayReceiveItem() );
					item->position( myTileGrid.tileCenter( pos ));
					addChildAt( item, afterTileGrid );
				}
			}
		}
		
		scheduleCallback( FRESH_CALLBACK( onTimeForPreparationWarning ), 20 );

		provideEssentials( m_minInitialMines, m_minInitialTorches, m_minInitialTurrets );
	}
	
	vec2 ldWorld::findOpenItemSpawnPosition() const
	{
		while( true )
		{
			const vec2 destination = randInRange( LEGAL_BOUNDS.ulCorner(), LEGAL_BOUNDS.brCorner() );
			if( tileAt( destination ).mayReceiveItem() )
			{
				return destination;
			}
		}
		ASSERT( false );
		return vec2::ZERO;
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

	SmartPtr< const Human > ldWorld::player() const
	{
		return const_cast< ldWorld* >( this )->player();
	}

	void ldWorld::update()
	{
		maybeSpawnMonsters();
		
		Super::update();
		
		// TimeServer:
		updateScheduledCallbacks();

		updateActorCollisions();
		
		pickActiveHuman();

		if( isGameActive() )
		{
			m_playerHasMoved = m_playerHasMoved || ( player() && !player()->stepDirection().isZero() );
			
			++m_lastActiveUpdate;
		}
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
	
	SmartPtr< Item > ldWorld::createItemNear( const ClassInfo& itemClass, const vec2& pos )
	{
		const auto& myTileGrid = tileGrid();
		
		const size_t afterTileGrid = getChildIndex( &myTileGrid ) + 1;
		
		vec2 offset( 0, -WORLD_PER_TILE );
		
		for( int i = 0; i < 4; ++i )
		{
			const auto spawnPos = pos + offset;
			const auto& nearTile = tileAt( spawnPos );
			if( nearTile.mayReceiveItem() )
			{
				auto item = createObject< Item >( itemClass );
				item->position( snapToGrid( spawnPos ));
				addChildAt( item, afterTileGrid );
				return item;
			}
			offset.quickRot90();
		}
		
		return nullptr;
	}
	
	void ldWorld::pickActiveHuman()
	{
		if( m_playerControllerClass )
		{
			Human::ptr currentPlayer = player();
			
			if( currentPlayer )
			{
				if( !currentPlayer->alive() || currentPlayer->isPickedUp() )
				{
					// Can't use this one anymore.
					//
					currentPlayer->controller( nullptr );
					ASSERT( !currentPlayer->isPlayer() );
					ASSERT( !player() );
					currentPlayer = nullptr;
				}
			}
			
			// Do we need a new player?
			//
			if( !currentPlayer )
			{
				// Pick one.
				//
				forEachChild< Human >( [&]( Human& human )
									  {
										  if( human.alive() && !human.isPickedUp() )
										  {
											  currentPlayer = &human;
										  }
									  } );
				
				if( currentPlayer )
				{
					currentPlayer->controller( createObject< PlayerController >( *m_playerControllerClass ));
					ASSERT( currentPlayer->isPlayer() );
					ASSERT( player() == currentPlayer );
				}
			}
		}
	}

	size_t ldWorld::currentSpawnPhase() const
	{
		return std::min( PHASE_SPAWN_DELAY_RANGE.size() - 1, static_cast< size_t >(( timePlayedSeconds() - INITIAL_SETUP_TIME_SECONDS ) / SECONDS_PER_PHASE ));
	}
	
	void ldWorld::maybeSpawnMonsters()
	{
		// Don't spawn for the first few minutes.
		//
		if( timePlayedSeconds() < INITIAL_SETUP_TIME_SECONDS )
		{
			return;
		}
		
		const auto phase = currentSpawnPhase();
	
		if( !hasScheduledCallback( FRESH_CALLBACK( onTimeToProvide )))
		{
			scheduleCallback( FRESH_CALLBACK( onTimeToProvide ), 30 );
		}

		// Maybe announce new phase.
		//
		if( phase != m_lastSpawnPhase )
		{
			// Movin' on up.
			//
			stage().as< AppStage >()->hud().showMessage( createString( "Wave " << ( phase + 1 )), "NewWaveMessagePopup" );
			
			// Schedule the first spawn of the new phase.
			//
			m_nextSpawnTime = timePlayedSeconds() + 10;
		}
		m_lastSpawnPhase = phase;
		
		// So should we spawn now?
		//
		if( m_nextSpawnTime >= 0 && timePlayedSeconds() >= m_nextSpawnTime )
		{
			// Yes.
			//
			spawnMonster();
			
			const auto& range = PHASE_SPAWN_DELAY_RANGE.at( phase );
			m_nextSpawnTime = timePlayedSeconds() + range.first + randInRange( -range.second, range.second );
		}
	}
	
	void ldWorld::spawnMonster()
	{
		static const std::vector< ClassWeights > PHASE_CLASS_WEIGHTS =
		{
			// 0
			{
				{ getClass( "Troll" ), 1 },
			},
			// 1
			{
				{ getClass( "Troll" ), 1 },
			},
			// 2
			{
				{ getClass( "Troll" ), 1 },
			},
			// 3
			{
				{ getClass( "Troll" ), 1 },
			},
			// 4
			{
				{ getClass( "Troll" ), 1 },
			},
		};
		ASSERT( PHASE_CLASS_WEIGHTS.size() == PHASE_SPAWN_DELAY_RANGE.size() );

		// Maybe spawn monsters.
		//
		const auto& classWeights = PHASE_CLASS_WEIGHTS.at( currentSpawnPhase() );
		if( !classWeights.empty() )
		{
			auto monsterClass = randomClass( classWeights );
			if( monsterClass )
			{
				stage().as< AppStage >()->hud().showMessage( "A Monster approaches.", "MonsterMessagePopup" );
				
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

	FRESH_DEFINE_CALLBACK( ldWorld, onTimeToProvide, fr::Event )
	{
		provideEssentials( 1, 2, 1 );
		
		scheduleCallback( FRESH_CALLBACK( onTimeToProvide ), 30 );
	}

	void ldWorld::provideEssentials( size_t minMines, size_t minTorches, size_t minTurrets )
	{
		// Ensure that at least a minimum number of weapons have been spawned.
		//
		ensureAtLeast< Mine >( minMines, "MineConfigured" );
		ensureAtLeast< Torch >( minTorches, "TorchConfigured" );
		ensureAtLeast< Turret >( minTurrets, "TurretConfigured" );
	}
	
	FRESH_DEFINE_CALLBACK( ldWorld, onTimeForPreparationWarning, fr::Event )
	{
		stage().as< AppStage >()->hud().showMessage( "Monsters will come.\nPrepare your defenses.", "PreparationWarning" );
	}

}

