
// named-results.go
package main

import "fmt"

func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return  // naked return, x와 y를 반환
}

func main() {
	fmt.Println(split(17))
}

