from django.conf.urls import url
from django.contrib import admin
from polls import views

urlpatterns = [
	url( r'^$', views.index, name = 'index' ),
	url( r'^(?P<aQuestion_id>\d+)/$', views.detail, name = 'detail' ),
	url( r'^(?P<aQuestion_id>\d+)/vote/$', views.vote, name = 'vote' ),
	url( r'^(?P<aQuestion_id>\d+)/results/$', views.results, name = 'results' ),
]

