#include <iostream>
#include <cstring>

using namespace std;

#define MAX_STR 255
#define MIN(a, b) ( a < b ? a : b )

#define ASCII (0)
#define TEX   (1)

/* Product */
/* 생성할 복합 객체 */
class Text
{
public:
	void setType( int aType ) { _type = aType; };
	void setText( char * aText ) { _text = aText; };

private :
	int _type;
	char * _text;
}

/* Director */
/* Builder 인터페이스를 사용하는 객체를 합성한다. */
class RTFReader()
{
public:
	RTFReader();

	void ParseRTF( char *aMsg, TextConverter * aConverter );
}

void RTFReader::ParseRTF( char *aMsg, TextConverter * aConverter )
{
	TEXT * sText = NULL;
	switch( aType )
	{
		case ASCII:
			sText = aConverter->getText();
			sText->convertCharactor( "ASCII Text\n" );
			break;
		case TEX:
			sText = aConverter=>getText();
			sText->convertCharactor( "TeX Text\n" );		
			break;
		default:
			break;
	}
}

/* Builder */
/* TextConverter 를 통해 Converter 를 만드는 각각의 요소를 생성하는 연산을 정의 */
class TextConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	
protected:
	TextConverter();
};

/* ConcreteBuilder */
class ASCIIConverter : public TextConverter
{
public:
	Text * getASCIIText()
	{
		_ASCIIText = new Text( ASCII );

		return _ASCIIText;
	}

	void converterCharactor( char *aChar )
	{
		int len = 0;

		len = strlen( aChar );
		len = MIN( len, MAX_STR );

		memcpy( _ASCIIText->_text, aChar, len );
	}

private:
	TextConverter * _Converter;
	Text * _ASCIIText;
};

/* ConcreteBuilder */
class TeXConverter
{
public:
	Text * getTeXText()
	{
		_TeXText = new Text( TEX );

		return _TeXText;
	}

	void converterCharactor( char *aChar )
	{
		int len = 0;

		len = strlen( aChar );
		len = MIN( len, MAX_STR );

		memcpy( _TeXText->_text, aChar, len );
	}

	void convertParagraph( void ) { return 0 };

private:
	TextConverter * _Converter;
	Text * _TeXText;
};

int main( void )
{
	RTFReader sRtf;
	ASCIIConverter sAscii;
	TexConverter sTex;


	sRtf.ParseRTF( "hihi", &sAscii );
	sRtf.ParseRTF( "hihi", &sTex );

	

	return 0;
}
