from django.shortcuts import render
from polls.models import Question

def index( aRequest ):
	lastest_question_list = Question.objects.all().order_by( '-pub_date' )[:5]
	sContext = { 'lastest_question_list:': lastest_question_list }
	return render( aRequest, 'polls/index.html', sContext )

def detail( aRequest, aQuestion_id ):
	sQuestion = get_object_or_404( Question, pk = aQuestion_id )
	return render( aRequest, 'polls/detail.html', { 'question': sQuestion } )

def vote( aRequest, aQuestion_id ):
    sP = get_object_or_404( Question, pk = aQuestion_id )
	try:
		sSelect_choice = sP.choice_set.get( pk = aRequest.POST['choice'] )
	except:
	else:
