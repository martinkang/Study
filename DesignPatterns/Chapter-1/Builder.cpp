#include <iostream>
#include <cstring>

using namespace std;

#define MAX_STR 255
#define MIN(a, b) ( a < b ? a : b )

#define ASCII (0)
#define TEX   (1)

class RTFReader
{
public:
private:
}
class TextConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	virtual TextConverter * getConverter();

protected:
	TextConverter();
};

class ASCIIConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual TextConverter * getConverter();

private:
	char * getASCIITest() { return _ASCIIText; };
	char _ASCIIText[MAX_STR];
};

virtual TextConverter * ASCIIConverter::getConverter()
{
	return new ASCIIConverter();
}

void ASCIIConverter::converterCharactor( char *aChar )
{
	int len = 0;

	len = strlen( aChar );
	len = MIN( len, MAX_STR );

	memcpy( _ASCIIText, aChar, len );
}

class TeXConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	virtual TextConverter * getConverter();

private:
	char * getTextConverter() { return _TexText; };
	char _TexText[MAX_STR];
};

class RTFReader()
{
public:
	char * ParseRTF( char *aToken );

private:
}

char * RTFReader::ParseRTF( int aType, char *aMsg )
{
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
