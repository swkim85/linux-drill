#!/usr/bin/php
<?php

  $start = 100;
  $end = 10000000000;

  for ($n = $start ; $n < $end; $n = $n*10) {
    print<<<EOS
echo $n
export N=$n
gcc -fopenmp  -o serial_pi -DN=\$N serial_pi.c 
./serial_pi

EOS;
  }

  for ($n = $start ; $n < $end; $n = $n*10) {
    print<<<EOS
echo $n
export N=$n
gcc -fopenmp  -o parallel_pi  -DN=\$N -DNUM_THREADS=32  parallel_pi.c
./parallel_pi

EOS;
  }


?>
