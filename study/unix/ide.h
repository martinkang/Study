#define IDE_TRUE 1
#define IDE_FALSE 0

#define IDE_SUCCESS 0
#define IDE_FAILURE -1

#define IDE_CONT( __label ) goto __label

#define EXCEPTION_END __EXCEPTION_END:

#define IDE_TEST( __condition ) \
	if ( __condition < 0 )\
	  goto __EXCEPTION_END  \
	
#define IDE_TEST_RAISE( __condition, __label ) \
	if ( __condition == IDE_TRUE )\
	  goto __label  \

#define IDE_EXCEPTION( __label ) \
	goto __EXCEPTION_END;__label : 
