from django.contrib import admin
from polls.models import Question, Choice

class ChoiceInline( admin.StackedInline ):
    model = Choice
    extra = 2

class QuestionAdmin( admin.ModelAdmin ):
    fieldsets = [
        ( 'Question Statement', { 'fields': ['question_text' ] }),
        #( 'Date Information', { 'fields': ['pub_date'] } ) 
        ( 'Date Information', { 'fields': ['pub_date'], 'classes':['collapse']}) # collapase 는 필드 접기 기능 show - hide
    ]
    inlines = [ChoiceInline] # Choice 모델 같이 보기
    list_display = ('question_text', 'pub_date' ) # 레코드 리스트 항목 지정
    list_filter = ['pub_date'] # 필터 사이드바 추가
    search_fields = ['question_text'] # 검색 박스 추가
# Register your models here.

admin.site.register( Question, QuestionAdmin )
admin.site.register( Choice )
