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
	
	const real DAMPING = 100;
	const real DESIRED_DIST = 5;
	
	void constrainBodies( Body& a, Body& b )
	{
		const auto delta = b.position() - a.position();
		const auto dist = delta.length();
		
		const auto velDelta = b.velocity() - a.velocity();
		
		const auto stiffness = std::min( a.stiffness(), b.stiffness() );
		
		const auto force =
			calculateSpringForce( delta, dist, velDelta.dot( delta ), stiffness, DAMPING, DESIRED_DIST );
		
		a.applyImpulse( force );
		b.applyImpulse( -force );
	}
}

namespace ld
{	
	FRESH_DEFINE_CLASS( Scene )
	DEFINE_VAR( Scene, vec2, m_gravity );
	DEFINE_VAR( Scene, vec2, m_wind );
	DEFINE_VAR( Scene, std::vector< Link >, m_links );
	FRESH_IMPLEMENT_STANDARD_CONSTRUCTORS( Scene )
	
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

