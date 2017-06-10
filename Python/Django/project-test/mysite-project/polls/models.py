from django.db import models

# Question Table
#	Coulmn			Type			Django Var				Django Filed Class
#	 id				 integer		 ( question_id )		 ( PK 는 Django 에서 자동 생성 )
#	 question_text	 varchar(200)	 question_text			 models.CharFiled( max_length = 200 )
#	 pub_date		 datetime		 pub_date				 models.DateTimeFiled( 'date published' )
class Question( models.Model ):
	question_text = models.CharField( max_length = 200 )
	pub_date = models.DateTimeField( 'date published' )

	def __str__( self ) : # __unicode__ on Python 2
		return self.question_text

class Choice( models.Model ):
	# FK 는 항상 다른 테이블의 PK 만 연결되므로 Question 클래스의 question_id 까지 지정할 필요가 없다.
	question = models.ForeignKey( Question )
	choice_text = models.CharField( max_length = 200 )
	votes = models.IntegerField( default = 0 )

	def __str__( self ):
		return self.choice_text

# Create your models here.
