#ifndef ParserInfo_H
#define ParserInfo_H

#include <vector>
#include <iostream>

class Type;
class Node;

extern std::vector< Node * > nodeTarget;

class Value
{
	public:
		Value( void );
		virtual ~Value( void );


};

class IntValue : public Value
{
	public:
		IntValue( void );
		IntValue( int val );
		virtual ~IntValue( void );

		static Type getType( void );
		void set( int val );
		int get( void );

	private:
		int val;
};

class StringValue : public Value
{
	public:
		StringValue( void );
		StringValue( std::string val );
		virtual ~StringValue( void );

		static Type getType( void );
		void set( std::string val );
		std::string get( void );

	private:
		std::string val;
};

class FloatValue : public Value
{
	public:
		FloatValue( void );
		FloatValue( float val );
		virtual ~FloatValue( void );

		static Type getType( void );
		void set( float val );
		float get( void );
		
	private:
		float val;
};

class BoolVal : public Value
{
	public:
		BoolVal( void );
		BoolVal( bool val );
		virtual ~BoolVal( void );

		static Type getType( void );
		void set( bool val );
		bool get( void );

	private:
		bool val;
};

// TODO still needs work
class ObjVal : public Value
{
	public:
		ObjVal( void );
		ObjVal( void *val, Type type );
		~ObjVal( void );

		Type getType( void );
		void set( void *val );
		void *get( void );

		Value callMethod( std::string methodName, std::vector< Value * > params );

	private:
		void *val;
		Type type;
};

class Node
{
	public:
		Node( void );
		virtual ~Node( void );

		virtual Value execute( void ) = 0;
};

class ValueNode : public Node
{
	public:
		ValueNode( void );
		ValueNode( int val );
		ValueNode( float val );
		ValueNode( bool val );
		ValueNode( std::string val );
		ValueNode( void *val, Type type );
		virtual ~ValueNode( void );

		void set( int val );
		void set( float val );
		void set( bool val );
		void set( std::string val );
		void set( void *val );

		Value *getValue( void );

	private:
		Type type;
		Value value;
};

class SetNode : public Node
{
	public:
		SetNode( void );
		SetNode( std::string varName, ValueNode *value );
		virtual ~SetNode( void );

		void set( std::string varName, ValueNode *value );
		Value execute( void );

	private:
		std::string varName;
		ValueNode *value;

};

class FuncNode : public Node
{
	public:
		FuncNode( void );
		virtual ~FuncNode( void );

		Value execute( std::vector< Value * > params );

	private:
		Symbol *definition;
		std::vector< Node * > codebody;
};

class Type
{
	public:
		Type( void )
		{
			isFunction = false;	
		};
		Type( std::string name )
		{
			( *this ) = Type();
			this->name = name;	
		};
		~Type( void ) {};

		bool operator == ( Type &other )
		{
			if ( !isFunction )
			{
				return other.name == this->name;	
			} else
			{
				for ( size_t i = 0; i < paramTypes.size(); ++i )
				{
					if ( other.paramTypes[ i ] != paramTypes[ i ] )
					{
						return false;
					}
				}

				return other.returnType == returnType;
			}
		};

		bool operator != ( Type &other )
		{
			return !( (*this) == other );
		};

		static const Type TYPE_INT;
		static const Type TYPE_BOOL;
		static const Type TYPE_FLOAT;
		static const Type TYPE_STRING;

		void setData( std::string name )
		{
			this->name = name;
			isFunction = false;	
		};

		void setFunc( Type returnType )
		{
			isFunction = true;	
		};
		void addParam( Type paramType )
		{
			paramTypes.push_back( paramType );	
		};

	private:
		// For Data
		std::string name;

		// For functions
		bool isFunction;
		Typename returnType;
		std::vector< Typename > paramTypes;
};

class Symbol
{
	public:
		Symbol( void )
		{
			value = NULL;
			symbolTable.push_back( this );	
		};
		~Symbol( void )
		{
			if ( value != NULL )
			{
				delete value;
				value = NULL;
			}	
		};

		void set( Type type, std::string name )
		{
			this->type = type;
			this->name = name;	
		};

		void setValue( Node *value );

		static Symbol *getSymbol( std::string name );
		static void makeSymbol( Symbol *s );
		static void makeSymbol( Type type, std::string name, Node *value );

	private:
		Type type;
		std::string name;
		
		Node *value;

		static std::vector< Symbol * > symbolTable;
};
#endif /* ParserInfo_H */
