#pragma once

#include <string>
#include <cstddef>

struct Symbol
{
	std::string name;
	size_t length;

	inline Symbol( const std::string &nam, size_t len = 0 ) :
		name( nam ), length( len ) { }

	static inline Symbol FromSignature( const std::string &signature )
	{
		return Symbol( signature, signature.size( ) );
	}

	static inline Symbol FromName( const std::string &name )
	{
		return Symbol( "@" + name );
	}
};
