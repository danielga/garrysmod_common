#pragma once

#include <string>
#include <cstddef>
#include <cstdint>

struct Symbol
{
public:
	enum class Type
	{
		None,
		Signature,
		Name
	};

	const Type type = Type::None;
	const std::string name;
	const size_t length = 0;
	const intptr_t offset = 0;

	inline Symbol( ) = default;

	template<std::size_t Size>
	static inline Symbol FromSignature( const char ( &signature )[Size], const intptr_t offset = 0 )
	{
		return Symbol( Type::Signature, std::string( signature, Size - 1 ), offset );
	}

	static inline Symbol FromName( const std::string &name )
	{
		return Symbol( Type::Name, "@" + name, 0 );
	}

private:
	inline Symbol( Type _type, std::string &&_name, intptr_t _offset ) :
		type( _type ),
		name( std::move( _name ) ),
		length( type == Type::Signature ? name.size( ) : 0 ),
		offset( _offset )
	{ }
};
