# xml-parsing

# What is this

This repo comes from a school project.We were assigned to create a program that, using a xml parser of our choosing, would process and retrieve statistical information from Wikipedia xml snapshots. There are two projects: one developed in Java and another in C. They do the same task but were evaluated on different programming styles and mindsets. Generally speaking, the C version is,on average, twice as fast and the Java version is more readable and generally easier to expand on.

# What does it do

run the main program('program' in C-project and 'run.sh' in Java-project) and pass it any number of xml files. As is, the currently implemented interface code displays all the funcionalities available:

-Total number of articles
-Unique number of articles
-Unique number of revisions
-Top 10 contributors
-Top 20 largest articles (by number of bytes)
-Top N articles with more words
-List of articles with a certain prefix
-Given an article id, retrieve title.
-Given an article id and revision id, retrieve timestamp
-Given an user id, retrieve contributer name
