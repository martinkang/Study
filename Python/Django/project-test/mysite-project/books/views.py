from django.views.generic.base import TemplateView
from django.views.generic import ListView
from django.views.generic import DetailView
from books.models import Book, Author, Publisher

# Create your views here.

# TemplateView
class BooksModelView( TemplateView ):
    template_name = 'books/index.html'

    def get_context_data( self, **kwargs ):
        context = super( BooksModelView, self ).get_context_data( **kwargs )
        context['object_list'] = ['Book', 'Author', 'Publisher']
        return context


# ListView
class BookList( ListView ):
    model = Book

class AuthorList( ListView ):
    model = Author

class PublisherList( ListView ):
    model = Publisher


# DetailView
class BookDetail( DetailView ):
    model = Book

class AuthorDetail( DetailView ):
    model = Author

class PublisherDetail( DetailView ):
    model = Publisher
