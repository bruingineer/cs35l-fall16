I decided to parallized the function so that each thread renders a chunk of the
picture.  If the width is not divisible nicely by the number of threads, then 
the last thread picks up the slack.  So a 100 pixel width picture divided by 4
threads will have each thread render 25 columns. 


I kept having a seg fault and could nt find out where it was so I looked at each for loop
that was accessing an array.  It ended up being the last one that accessed the scaled
color array.  I fixed that by adding the width * height + py to access it correctly.

This variation of srt speeds up the process by double with 2 threads. with 4 threads,
it took a 3rd of the time. each added thread showed positvie yet diminishing results.
