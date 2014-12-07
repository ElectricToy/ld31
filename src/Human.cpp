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
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( Human )
	
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Human )

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
		return ( !isPlayer() && HOME_INNER_BOUNDS.doesEnclose( position() ) )
			? 1.0f
			: Super::currentStepSpeed();
	}

	void Human::updateAI()
	{
		if( alive() && !isPlayer() && !isPickedUp() )
		{
			real pctChanceToMove = 1;
			
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
				travelTo( destination );
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
}

