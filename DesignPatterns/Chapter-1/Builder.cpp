#include <iostream>
#include <cstring>

using namespace std;

#define MAX_STR 255
#define MIN(a, b) ( a < b ? a : b )

#define ASCII (0)
#define TEX   (1)

/* Product */
class Text
{
public:
	virtual void setType();

private :
	int _type;
	char text[MAX_STR];
}

/* Director */
class RTFReader
{
public:
private:
}

/* Builder */
class TextConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	virtual TextConverter * getConverter();

private:
	TextConverter * _Converter;
	Text * _Text;

protected:
	TextConverter();
};

/* ConcreteBuilder */
class ASCIIConverter
{
public:
	void convertCharactor( char * aChar );
	void convertFontChange( int aType );

	TextConverter * getConverter();

private:
	TextConverter * _Converter;
	Text * _ASCIIText;

	char * getASCIIText() { return _ASCIIText; };
	char _ASCIIText[MAX_STR]; /* Product */
};

TextConverter * ASCIIConverter::getConverter()
{
	_Converter = new ASCIIConverter;
	return _Converter;
}

void ASCIIConverter::converterCharactor( char *aChar )
{
	int len = 0;

	len = strlen( aChar );
	len = MIN( len, MAX_STR );

	memcpy( _ASCIIText, aChar, len );
}

/* ConcreteBuilder */
class TeXConverter
{
public:
	void convertCharactor( char * aChar );
	void convertFontChange( int aType );
	void convertParagraph( void );

	 TextConverter * getConverter();

private:
	 Text _TexText;
	 TextConverter * _Converter;

	char * getText() { return _TexText; };
	char _TexText[MAX_STR] = { "TexText\0" }; /* Product */
};

TextConverter * TeXConverter::getConverter()
{
	_Converter = new TeXConverter();
	return _TeXConverter;
}

class RTFReader()
{
public:
	char * ParseRTF( char *aToken );

private:
}

char * RTFReader::ParseRTF( int aType, char *aMsg )
{
	Converter * sConverter;

	char * sText = NULL;
	switch( aType )
	{
		case ASCII:
			
			break;
		case TEX:
			break;
		default:
			break;
	}
}

int main( void )
{
	return 0;
}
