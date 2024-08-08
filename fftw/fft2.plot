# gnuplot plot_commands.gnu
set terminal pngcairo enhanced color font 'Arial,10'
set title 'fft2 plot'
set xlabel 'X-axis'
set ylabel 'Y-axis'
set output 'fft2a.png'
plot 'fft2.txt' using 1:2 with linespoints title 'input(real)'
set output 'fft2b.png'
plot 'fft2.txt' using 1:5 with linespoints title 'output(imag)'
