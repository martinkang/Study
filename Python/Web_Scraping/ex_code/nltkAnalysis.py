# https://www.lucypark.kr/courses/2015-dm/text-mining.html 참고
from nltk import word_tokenize, sent_tokenize, pos_tag

sSentences = sent_tokenize("Google is one of the best companies in the world. I constantly google myself to see what I'm up to.")
sNouns = ['NN', 'NNS', 'NNP', 'NNPS'] # NN 명사들.  고유명사, 등등

for sentence in sSentences: 
	if "google" in sentence.lower(): 
		taggedWords = pos_tag(word_tokenize(sentence)) 
		for word in taggedWords: 
			# google 이 명사로 쓰인 경우만 출력
			if word[0].lower() == "google" and word[1] in sNouns: 
			print( sentence )
