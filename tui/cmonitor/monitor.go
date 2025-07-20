package cmonitor

/*
#cgo CFLAGS: -I${SRCDIR}
#cgo LDFLAGS: -L${SRCDIR} -lmonitor
#include "monitor.h"
*/
import "C"

func GetTotalMemoryKB() int64 {
	return int64(C.get_total_memory_kb())
}
