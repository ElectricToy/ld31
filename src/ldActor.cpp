//
//  ldActor.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/6/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "ldActor.h"
#include "ldTile.h"
#include "ldWorld.h"
#include "Stage.h"
#include "TileGrid.h"
#include "Creature.h"
#include "ParticleEmitter.h"
using namespace fr;

namespace ld
{	
	FRESH_DEFINE_CLASS( ldActor )
	DEFINE_DVAR( ldActor, vec2, m_carryOffset );
	DEFINE_DVAR( ldActor, vec2, m_carryScale );
	DEFINE_DVAR( ldActor, angle, m_carryRotation );
	DEFINE_DVAR( ldActor, vec2, m_precarryScale );
	DEFINE_DVAR( ldActor, real, m_health );
	DEFINE_DVAR( ldActor, real, m_maxHealth );
	DEFINE_DVAR( ldActor, bool, m_alive );
	DEFINE_DVAR( ldActor, bool, m_suppressesHolderLight );
	DEFINE_DVAR( ldActor, bool, m_dropsWhenUsed );
	DEFINE_DVAR( ldActor, bool, m_pushesHolderWhenUsed );
	DEFINE_DVAR( ldActor, int, m_desiredDepth );
	DEFINE_VAR( ldActor, WeakPtr< Creature >, m_holder );
	DEFINE_DVAR( ldActor, TimeType, m_diedTime );
	DEFINE_VAR( ldActor, ClassInfo::cptr, m_lightClass );
	DEFINE_DVAR( ldActor, real, m_lightWobble );
	DEFINE_DVAR( ldActor, real, m_lightWobbleLerp );
	DEFINE_DVAR( ldActor, real, m_lightRadius );
	DEFINE_DVAR( ldActor, Color, m_lightColor );
	DEFINE_VAR( ldActor, ClassInfo::cptr, m_dieEmitterClass );
	DEFINE_VAR( ldActor, std::string, m_friendlyName );

	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( ldActor )

	real ldActor::lightWobble() const
	{
		return m_lightWobble;
	}
	
	bool ldActor::mayCollide() const
	{
		return !isMarkedForDeletion() && !isPickedUp() && alive();
	}
	
	void ldActor::onAddedToStage()
	{
		Super::onAddedToStage();
		
		position( snapToGrid( position() ));
		recordPreviousState();
	}
	
	void ldActor::onTouched( ldActor& other )
	{}
	
	bool ldActor::isPickedUp() const
	{
		return m_holder != nullptr;
	}
	
	bool ldActor::canBePickedUp() const
	{
		return !isPickedUp() && alive();
	}
	
	bool ldActor::canBePickedUpByTouch() const
	{
		return canBePickedUp();
	}
	
	vec2 ldActor::bePickedUpBy( Creature& other )
	{
		ASSERT( !isPickedUp() );

		m_holder = &other;
		
		rotation( m_carryRotation );
		scale( m_carryScale );
		
		return m_carryOffset;
	}
	
	void ldActor::beDroppedBy( Creature& other )
	{
		rotation( 0 );
		scale( 1 );
		m_holder = nullptr;
	}
	
	void ldActor::beUsedBy( Creature& other )
	{
		if( m_dropsWhenUsed )
		{
			beDroppedBy( other );
		}
	}
	
	ldWorld& ldActor::world() const
	{
		const auto ancestor = firstAncestorOfType< ldWorld >( *this );
		ASSERT( ancestor );
		return *ancestor;
	}
	
	TileGrid& ldActor::tileGrid() const
	{
		return world().tileGrid();
	}
	
	void ldActor::update()
	{
		Super::update();
		
		if( m_lightClass && m_lightRadius > 0 && m_lightColor != Color::Invisible )
		{
			const auto desiredLightRadius = ( suppressesHolderLight() && isPickedUp() ) ? 240.0f : m_lightRadius;
			
			if( !m_lightSource )
			{
				m_lightSource = createObject< LightSource >( *m_lightClass );
			
				m_lightSource->radius( desiredLightRadius );
				
				if( world().lighting() )
				{
					world().lighting()->addChild( m_lightSource );
				}

				world().attach( *m_lightSource, *this );
			}
			
			m_lightSource->radius( lerp( m_lightSource->radius(), desiredLightRadius + randInRange( 0.0f, lightWobble() ), m_lightWobbleLerp ));
			m_lightSource->color( m_lightColor );
		}
		else
		{
			if( m_lightSource )
			{
				m_lightSource->destroyWithAnimation();
				m_lightSource = nullptr;
			}
		}
	}

	void ldActor::receiveDamage( real amount )
	{
		ASSERT( amount >= 0 );
		
		if( alive() )
		{
			m_health -= amount;
			
			if( m_health <= 0 )
			{
				die();
			}
		}
	}
	
	void ldActor::die()
	{
		if( alive() )
		{
			m_alive = false;
			
			m_diedTime = world().time();
			
			if( m_lightSource )
			{
				m_lightSource->destroyWithAnimation();
				m_lightSource = nullptr;
			}

			if( m_dieEmitterClass )
			{
				auto particles = createObject< ParticleEmitter >( *m_dieEmitterClass );
				particles->position( position() );
				parent()->addChild( particles );
			}
		}
	}
	
	int ldActor::desiredDepth() const
	{
		if( m_holder )
		{
			return m_holder->desiredDepth() + 1;
		}
		else if( alive() )
		{
			return m_desiredDepth;
		}
		else
		{
			return -2000;
		}
	}
}

