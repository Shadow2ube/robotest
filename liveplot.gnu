# set xrange [0:100]
set yrange [-1:1]
plot "velocity.csv" using 1:2 with lines
pause 1
reread
