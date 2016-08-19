#include <iostream>
#include <cstring>

#define MAX_STR 255
#define MIN(a, b) ( a < b ? a : b )
using namespace std;

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

class TextWidgetConverter()
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	virtual TextConverter * getConverter();

private:
	char * getTextWidget() { return _TextWidget; };
	
	char _TextWidget[10];
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
		case TextConverter:
			break;
		case ASCIIConverter:
			break;
		case TextWidgetConverter:
			break;
		default:
			break;
	}
}

int main( void )
{
	return 0;
}
