#include <iostream>

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
	char _ASCIIText[10];
};

class TeXConverter
{
public:
	virtual void convertCharactor( char * aChar );
	virtual void convertFontChange( int aType );
	virtual void convertParagraph( void );

	virtual TextConverter * getConverter();

private:
	char * getTextConverter() { return _TexText; };
	char _TexText[10];
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
