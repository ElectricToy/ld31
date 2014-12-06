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
	
	void constrainBodies( Body& a, Body& b, real restDistance )
	{
		const auto delta = b.position() - a.position();
		const auto dist = delta.length();
		
		if( dist > 0 )
		{
			const auto normal = delta / dist;
			
			const auto halfRestDelta = ( restDistance - dist ) * 0.5f;
			
			const auto displacement = normal * halfRestDelta * 1.f;
			
			const auto totalMass = a.effectiveMass() + b.effectiveMass();
			
			const auto proportionA = b.effectiveMass() / totalMass;
			
			a.position( a.position() - displacement * proportionA );
			b.position( b.position() + displacement * ( 1.0f - proportionA ));
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
		m_links.emplace_back( a, b, distance( a->position(), b->position() ));
	}
	
	void Scene::onAddedToStage()
	{
		Super::onAddedToStage();
		
		for( int y = 0; y < 24; ++y )
		{
			for( int x = 0; x < 6; ++x )
			{
				auto body = createObject< Body >( *getClass( "BodyDefault" ));
				
				body->teleport( vec2( x * 5 + y * 3, DEEP_GROUND_Y + 60 - y * 20 ) + makeRandomVector2( 4.0f ));

				size_t bodiesToLink = randInRange( 8,8 );
				for( size_t iChild = numChildren() - 1; iChild < numChildren(); --iChild )
				{
					if( auto other = getChildAt( iChild )->as< Body >())
					{
						if( pctChance( 30 ))
						{
							linkBodies( body, other );
							--bodiesToLink;
							
							if( bodiesToLink == 0 )
							{
								break;
							}
						}
					}
				}
				
				addChild( body );
			}
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
									  [&]( const Link& link ) { return !std::get< 0 >( link ) || !std::get< 1 >( link ); } ),
										  m_links.end() );
										  

		for( int i = 0; i < 20; ++i )
		{
			for( const auto& link : m_links )
			{
				constrainBodies( *std::get< 0 >( link ), *std::get< 1 >( link ), std::get< 2 >( link ));
			}
		}
	}
	
	
}

