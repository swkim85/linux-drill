
# a=a+1
for {set a 1} {$a < 11} {set a [expr $a + 1]} {
puts $a
}

