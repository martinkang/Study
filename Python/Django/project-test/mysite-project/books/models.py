from django.db import models

# Create your models here.
class Book( models.Model ):
    title = models.CharField( max_length = 100 )
    authors = models.ManyToManyField( 'Author' )
    publisher = models.ForeignKey( 'Publisher' )
    publication_date = models.DateField()

    def __str__( self ):
        return self.title

class Author( models.Model ):
    salutation = models.CharField( max_length = 100 )
    name = models.CharField( max_length = 50 )
    email = models.EmailField()

    def __str__( self ):
        return self.name

class Publisher( models.Model ):
    name = models.CharField( max_length = 100 )
    address = models.CharField( max_length = 100 )
    website = models.URLField()

    def __str__( self ):
        return self.name