import cluster

sBlogNames, sWords, sData = cluster.readfile( 'blogdata.txt' )

sClust = cluster.hcluster( sData )
cluster.printclust( sClust, aLabels = sBlogNames )
cluster.drawdendrogram( sClust, sBlogNames, 'blogclust.jpg' )
