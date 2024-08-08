# gnuplot plot_commands.gnu
set terminal pngcairo enhanced color font 'Arial,10'
set title 'fft1 plot'
set xlabel 'X-axis'
set ylabel 'Y-axis'
set output 'fft1a.png'
plot 'fft1.txt' using 1:2 with linespoints title 'input(real)'
set output 'fft1b.png'
plot 'fft1.txt' using 1:5 with linespoints title 'output(imag)'
