#!/usr/bin/php
<?php
  error_reporting(0);

for ($i = 409600; $i <= 4096000; $i+= 1024) {
  print<<<EOS

echo $i
gcc -g  -o add1  add1.c  -DARRAY_LENGTH=$i
./add1


EOS;

}


