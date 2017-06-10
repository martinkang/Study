from django.shortcuts import render
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from polls.models import Choice, Question


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
	except ( KeyError, Choice.DoesNotExist ):
		return render( aRequest, 'poll/detail.html',
		{ 'question' : sP,
		'error_message': "You didn`t select a choice." 
		} )
	else:
		sSelect_choice.votes += 1
		sSelect_choice.save()
		return HttpResponseRedirect( reverse( 'polls:results', args = ( sP.id, )))

def results( aRequest, aQuestion_id ):
	sQuestion = get_object_or_404( Question, pk = aQuestion_id )
	return render( aRequest, 'pools/requests.html', {'question': sQuestion } )
