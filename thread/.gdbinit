
set pagination off
set confirm off

define stepi_n
  set $i = 0
  while $i < $arg0
    si
    set $i = $i + 1
  end
end

#display /x $pc



