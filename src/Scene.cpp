//
//  Scene.cpp
//  FreshApp
//
//  Created by Jeff Wofford on 12/5/14.
//  Copyright (c) 2014 Jeff Wofford. All rights reserved.
//

#include "Scene.h"
using namespace fr;

namespace
{
	using namespace ld;
	
	const real DAMPING = 0;
	const real DESIRED_DIST = 20;
	
	void constrainBodies( Body& a, Body& b )
	{
		const auto stiffness = std::min( a.stiffness(), b.stiffness() );
		
		// Constrain position.
		//
		{
			const auto delta = b.position() - a.position();
			
			const auto velDelta = b.velocity() - a.velocity();
			
			const auto force =
				calculateSpringForceVector( delta, velDelta, stiffness, DAMPING, DESIRED_DIST );
			
			a.applyImpulse( force );
			b.applyImpulse( -force );
		}
		
		// Constrain rotation.
		//
		{
			const auto delta = b.rotation() - a.rotation();
			const auto dist = fr::abs( delta );
			
			const auto velDelta = b.angularVelocity() - a.angularVelocity();
			
			const auto force =
				calculateSpringForce( delta, dist, velDelta, delta * velDelta, (angle) stiffness, (angle) DAMPING );
			
			a.applyTorque( force );
			b.applyTorque( -force );
		}
	}
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Scene )
	DEFINE_VAR( Scene, vec2, m_gravity );
	DEFINE_VAR( Scene, vec2, m_wind );
	DEFINE_VAR( Scene, std::vector< Link >, m_links );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Scene )
	
	void Scene::linkBodies( Body::ptr a, Body::ptr b )
	{
		ASSERT( a && b );
		ASSERT( a != b );
		m_links.push_back( std::make_pair( a, b ));
	}
	
	void Scene::onAddedToStage()
	{
		Super::onAddedToStage();
		
		for( int i = 0; i < 40; ++i )
		{
			auto body = createObject< Body >( *getClass( "BodyDefault" ));
			
			body->teleport( vec2( 0, DEEP_GROUND_Y - ( i * 5.0f )) + makeRandomVector2( 20.0f ) );

			size_t bodiesToLink = randInRange( 2, 4 );
			for( size_t iChild = numChildren() - 1; iChild < numChildren(); --iChild )
			{
				if( auto other = getChildAt( iChild )->as< Body >())
				{
					linkBodies( body, other );
					--bodiesToLink;
					
					if( bodiesToLink == 0 )
					{
						break;
					}
				}
			}
			
			addChild( body );
		}
	}
	
	void Scene::update()
	{
		eachBody( [&]( Body& body )
				 {
					 body.applyGravity( m_gravity );
					 body.applyWind( m_wind );
				 } );
		
		Super::update();

		m_links.erase( std::remove_if(
									  m_links.begin(),
									  m_links.end(),
									  [&]( const Link& link ) { return !link.first || !link.second; } ),
										  m_links.end() );
										  

		for( const auto& link : m_links )
		{
			constrainBodies( *link.first, *link.second );
		}
	}
	
	
}

