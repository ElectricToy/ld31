//
//  Human.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Human.h"
#include "AppStage.h"
#include "HUD.h"
#include "ldWorld.h"
#include "ldTile.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Human )
	DEFINE_DVAR( Human, int, m_controlPriority );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Human )

	int Human::desiredDepth() const
	{
		return Super::desiredDepth() + ( isPlayer() ? 10 : 0 );
	}
	
	real Human::lightWobble() const
	{
		if( !isPlayer() )
		{
			return 0;
		}
		else
		{
			return Super::lightWobble();
		}
	}
	
	real Human::normalLightRadius() const
	{
		if( !isPlayer() )
		{
			return 0;
		}
		else
		{
			return Super::normalLightRadius();
		}
	}
	
	bool Human::canPickup( const ldActor& other ) const
	{
		return isPlayer() && other.isItem() && Super::canPickup( other );
	}
	
	bool Human::isPlayer() const
	{
		return controller() != nullptr;
	}
	
	bool Human::canStep( const vec2& dir ) const
	{
		return Super::canStep( dir ) && LEGAL_BOUNDS.doesEnclose( snapToGrid( position() + dir * WORLD_PER_TILE ));
	}
	
	real Human::currentStepSpeed() const
	{
		// Non-player humans are slower at home.
		//
		if( isPlayer() )
		{
			return Super::currentStepSpeed();
		}
		else if( HOME_INNER_BOUNDS.doesEnclose( position() ))
		{
			return 1.0f;
		}
		else
		{
			return 3.0f;
		}
	}

	real Human::grindDamage() const
	{
		return isPlayer() ? Super::grindDamage() : 0;
	}
	
	void Human::updateAI()
	{
		if( alive() && !isPlayer() && !isPickedUp() )
		{
			real pctChanceToMove = 1;
			
			// Wander.
			
			// At home?
			//
			if( !HOME_INNER_BOUNDS.doesEnclose( position() ) &&
			    !HOME_INNER_BOUNDS.doesEnclose( travelDestination() ))
			{
				// Nope. Definitely move.
				//
				pctChanceToMove = 100;
			}
			
			if( pctChance( pctChanceToMove ))
			{
				// Go to a random location at home.
				//
				auto destination = randInRange( HOME_INNER_BOUNDS.ulCorner(), HOME_INNER_BOUNDS.brCorner() );
				const auto& proposedTile = world().tileAt( destination );
				if( !proposedTile.isSolid() )
				{
					auto item = world().itemInTile( destination );
					if( !item )
					{
						const vec2 tileUL = tileGrid().tileUL( tileGrid().worldToTileSpace( destination ));
						rect tileRect( tileUL, tileUL + WORLD_PER_TILE );
						
						// Another other actors here?
						//
						bool actorsHere = false;
						world().touchingActors( tileRect, [&]( const ldActor& actor )
															  {
																  actorsHere = actorsHere || ( actor.alive() );
															  } );
						
						if( !actorsHere )
						{
							travelTo( destination );
						}
					}
				}
			}
		}
	}
	
	void Human::die()
	{
		if( hasStage() )
		{
			stage().as< AppStage >()->hud().showMessage( createString( friendlyName() << " has perished." ), "MonsterMessagePopup" );
		}
		
		Super::die();
	}
	
	vec2 Human::bePickedUpBy( Creature& other )
	{
		if( hasStage() )
		{
			stage().as< AppStage >()->hud().showMessage( createString( friendlyName() << " has been captured." ), "MonsterMessagePopup" );
		}
		
		return Super::bePickedUpBy( other );
	}
	
	void Human::beDroppedBy( Creature& other )
	{
		if( hasStage() && alive() )
		{
			stage().as< AppStage >()->hud().showMessage( createString( friendlyName() << " has been rescued." ), "MonsterMessagePopup" );
		}
		
		Super::beDroppedBy( other );
	}

}

