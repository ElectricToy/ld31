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
#include "FreshPlayerController.h"
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

	const int INITIAL_SETUP_TIME_SECONDS = 60;
	const int SECONDS_PER_PHASE = 120;
	
	const TimeType SECONDS_PER_COUNT = 2.5;
	const int NUM_COUNTS = 5;
	const TimeType COUNTDOWN_INITIAL_DELAY = 4;
	
	const TimeType PROVISIONING_INTERVAL = 20;
	
	const std::vector< std::pair< int, int >> PHASE_SPAWN_DELAY_RANGE =
	{
		{ 40, 10 },		// seconds, delta
		{ 35, 10 },
		{ 20, 5 },
		{ 12, 5 },
		{ 8, 2 },
	};
}

namespace ld
{	
	FRESH_DEFINE_CLASS( ldWorld )
	DEFINE_VAR( ldWorld, int, m_lastActiveUpdate );
	DEFINE_VAR( ldWorld, bool, m_playerHasMoved );
	DEFINE_VAR( ldWorld, size_t, m_minInitialTorches );
	DEFINE_VAR( ldWorld, size_t, m_minInitialTurrets );
	DEFINE_VAR( ldWorld, size_t, m_minInitialMines );
	DEFINE_VAR( ldWorld, int, m_nextSpawnTime );
	DEFINE_VAR( ldWorld, int, m_countdown );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTOR_INERT( ldWorld )
	
	FRESH_CUSTOM_STANDARD_CONSTRUCTOR_NAMING( ldWorld )
	{}
	
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
		
		// Play music.
		//
		
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

		scheduleCallback( FRESH_CALLBACK( onTimeForCountdown ), INITIAL_SETUP_TIME_SECONDS - ( SECONDS_PER_COUNT * NUM_COUNTS + COUNTDOWN_INITIAL_DELAY ));

		provideEssentials( m_minInitialMines, m_minInitialTorches, m_minInitialTurrets );
	}
	
	vec2 ldWorld::findOpenItemSpawnPosition() const
	{
		while( true )
		{
			const vec2 destination = randInRange( ITEM_SPAWN_BOUNDS.ulCorner(), ITEM_SPAWN_BOUNDS.brCorner() );
			if( tileAt( destination ).mayReceiveItem() )
			{
				return destination;
			}
		}
		ASSERT( false );
		return vec2::ZERO;
	}
	
	FreshTileGrid& ldWorld::tileGrid() const
	{
		const auto child = getChildByName< FreshTileGrid >( "" );
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
		
		updateChildDepths();
		
		if( auto cam = camera() )
		{
			cam->setTarget( player() );
			cam->setClampingBounds( tileGrid().getMinimalBoundsWorldSpace( -8 ) );
			cam->setScreenSize( stage().stageDimensions() );

			Super::updateCamera();
		}
		
		// Update listener position.
		//
		if( AudioSystem::ready() )
		{
			auto viewBounds = globalToLocal( stage().stageBounds() );
			AudioSystem::instance().setListenerPosition( viewBounds.midpoint() );
		}
	}
	
	void ldWorld::updateChildDepths()
	{
		// We have three kinds of children:
		// 1. the tile grid, always child #0.
		// 2. the lighting, always the top child.
		// 3. ldActors, which go between, and are sorted by their desiredDepth().
		// We use swapChildren to achieve ordering, since it avoids the callbacks associated with add/removeChild().
		//
		
		ASSERT( numChildren() > 0 );
		
		FreshTileGrid::ptr myTileGrid = &tileGrid();
		Lighting::ptr myLighting = lighting();

		sortChildren( [&]( DisplayObject::ptr a, DisplayObject::ptr b )
					 {
						 if( a == myTileGrid || b == myLighting )
						 {
							 return true;
						 }
						 else if( b == myTileGrid || a == myLighting )
						 {
							 return false;
						 }
						 else
						 {
							 auto actorA = a->as< ldActor >();
							 auto actorB = b->as< ldActor >();
							 
							 if( !actorA && !actorB )
							 {
								 // We can't use their current depth. Just use the object address.
								 //
								 return a < b;
							 }
							 else if( !actorA )
							 {
								 return false;
							 }
							 else if( !actorB )
							 {
								 return true;
							 }
							 else
							 {
								 if( actorA->desiredDepth() == actorB->desiredDepth() )
								 {
									 return a < b;
								 }
								 else
								 {
									 return actorA->desiredDepth() < actorB->desiredDepth();
								 }
							 }
						 }
					 } );
	}
	
	void ldWorld::updateActors( const std::vector< SmartPtr< FreshActor > >& actors )
	{
		// Do nothing.
	}
	
	void ldWorld::onTapped( const fr::EventTouch& event )
	{
		if( auto thePlayer = player() )
		{
			thePlayer->controller()->travelTo( event.location() );
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
		Human::ptr currentPlayer = player();
		
		const bool hadPlayer = currentPlayer != nullptr;
		
		if( currentPlayer )
		{
			if( !currentPlayer->alive() || currentPlayer->isPickedUp() )
			{
				// Can't use this one anymore.
				//
				currentPlayer->isPlayer( false );
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
										  if( !currentPlayer || human.controlPriority() > currentPlayer->controlPriority() )
										  {
											  currentPlayer = &human;
										  }
									  }
								  } );
			
			if( currentPlayer )
			{
				currentPlayer->isPlayer( true );
				ASSERT( currentPlayer->isPlayer() );
				ASSERT( player() == currentPlayer );
				
				if( auto cam = camera() )
				{
					cam->setTarget( currentPlayer );
				}
				
				// We only do this on later characters because otherwise it's too noisy at the beginning.
				//
				if( hadPlayer )
				{
					stage().as< AppStage >()->hud().showMessage( createString( "You are " << currentPlayer->friendlyName() << "." ), "NewHumanMessagePopup" );
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
		if( !hasScheduledCallback( FRESH_CALLBACK( onTimeToProvide )))
		{
			scheduleCallback( FRESH_CALLBACK( onTimeToProvide ), PROVISIONING_INTERVAL );
		}
		
		// Don't spawn for the first few minutes.
		//
		if( timePlayedSeconds() < INITIAL_SETUP_TIME_SECONDS )
		{
			return;
		}
		
		const auto phase = currentSpawnPhase();
	
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
			// TODO!!!
//			spawnMonster();
			
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
				playSound( "alert01" );
				
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
		provideEssentials( 1, 3, 2 );
		
		scheduleCallback( FRESH_CALLBACK( onTimeToProvide ), PROVISIONING_INTERVAL );
	}

	void ldWorld::provideEssentials( size_t minMines, size_t minTorches, size_t minTurrets )
	{
		// Ensure that at least a minimum number of weapons have been spawned.
		//
		provideAtLeast< Mine >( minMines, "MineConfigured" );
		provideAtLeast< Torch >( minTorches, "TorchConfigured" );
		provideAtLeast< Turret >( minTurrets, "TurretConfigured" );
	}
	
	FRESH_DEFINE_CALLBACK( ldWorld, onTimeForPreparationWarning, fr::Event )
	{
		stage().as< AppStage >()->hud().showMessage( "Monsters will come.\nPrepare your defenses.", "PreparationWarning" );
	}

	FRESH_DEFINE_CALLBACK( ldWorld, onTimeForCountdown, fr::Event )
	{
		stage().as< AppStage >()->hud().showMessage( "Get Ready!", "PreparationWarning" );
		
		scheduleCallback( FRESH_CALLBACK( onTimeToShowCount ), COUNTDOWN_INITIAL_DELAY );
	}
	
	FRESH_DEFINE_CALLBACK( ldWorld, onTimeToShowCount, fr::Event )
	{
		const int count = NUM_COUNTS - m_countdown;
		
		stage().as< AppStage >()->hud().showMessage( createString( count << "..." ), "PreparationWarning" );
		
		++m_countdown;
		if( m_countdown < NUM_COUNTS )
		{
			scheduleCallback( FRESH_CALLBACK( onTimeToShowCount ), SECONDS_PER_COUNT );
		}
	}

}

