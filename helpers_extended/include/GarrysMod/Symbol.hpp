#pragma once

#include <string>
#include <cstddef>

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

	inline Symbol( ) = default;

	template<std::size_t Size>
	static inline Symbol FromSignature( const char ( &signature )[Size] )
	{
		return Symbol( Type::Signature, std::string( signature, Size - 1 ) );
	}

	static inline Symbol FromName( const std::string &name )
	{
		return Symbol( Type::Name, "@" + name );
	}

private:
	inline Symbol( Type _type, std::string &&_name ) :
		type( _type ), name( std::move( _name ) ), length( type == Type::Signature ? name.size( ) : 0 ) { }
};
