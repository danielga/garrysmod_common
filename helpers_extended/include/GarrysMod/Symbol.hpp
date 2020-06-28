#pragma once

#include <string>
#include <cstddef>

struct Symbol
{
	enum class Type
	{
		None,
		Signature,
		Name
	};

	const Type type = Type::None;
	const std::string name;
	const size_t length = 0;

	inline Symbol( ) { }

	inline Symbol( Type typ, const std::string &nam, size_t len = 0 ) :
		type( typ ), name( nam ), length( len ) { }

	static inline Symbol FromSignature( const std::string &signature )
	{
		return Symbol( Type::Signature, signature, signature.size( ) );
	}

	static inline Symbol FromName( const std::string &name )
	{
		return Symbol( Type::Name, "@" + name );
	}
};
