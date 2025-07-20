package main

import (
	"fmt"
	"tui/cmonitor"
)

func main() {
	memKB := cmonitor.GetTotalMemoryKB()

	if memKB < 0 {
		fmt.Println("Failed to read memory Info")
	} else {
		fmt.Printf("Total Memory: %d KB (%.2f GB)\n", memKB, (float64)(memKB)/(1024*1024))
	}
}
