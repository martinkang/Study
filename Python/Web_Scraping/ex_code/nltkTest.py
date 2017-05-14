#ImportError: No module named '_sqlite3'"
#=> python folder ./configure --enable-loadable-sqlite-extensions && make && sudo make install

from nltk import word_tokenize
from nltk import sent_tokenize
from nltk import Text
from nltk.book import * # nltk 에 내장된 책 
from nltk import FreqDist # 단어 빈도수
from nltk import bigrams
from nltk import ngrams
from nltk import pos_tag # 사전적 의미 분석

sTokens = word_tokenize( "Here is some not very interesting text" )
sTxt = Text( sTokens )
print( sTxt )

sFdist = FreqDist( text6 ) # text6 에서 단어 빈도수 출력
print( sFdist )
print( sFdist["ARTHUR"] ) # ARTHUR 의 빈도수 출력

sBigrams = bigrams( text6 )
sBigramsDist = FreqDist( sBigrams )
print( sBigramsDist )
print( sBigramsDist[("Sir", "Robin" )] )

sFourGrams = ngrams( text6, 4 )
for fourgram in sFourGrams:
	if fourgram[0] == "coconut":
		print( fourgram )

sWords = pos_tag( text6 )
print( sWords )


sSentences = sent_tokenize( "Google is one of the best companies in the world. I constantly google myself to see what I`m up to." )
sNouns = ['NN', 'NNS', 'NNP', 'NNPS' ] # NNP 고유명사

for sentence in sSentences:
	if "google" in sentence.lower():
		taggedWords = pos_tag( word_tokenize( sentence ) )

		for word in taggedWords:
			if word[0].lower() == "google" and word[1] in sNouns: # 구글이 명사로 쓰인 문장만 출력
				print( sentence )
