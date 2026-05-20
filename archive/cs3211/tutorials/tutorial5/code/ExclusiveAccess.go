package main

import (
	"fmt"
	"sync"
)

func main() {
	ch := make(chan int, 1) // make "unbuffered" channel
	var wg sync.WaitGroup
	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			count := <-ch // blocking dequeue
			count++       // safely add 1 as the exclusive owner
			ch <- count   // blocking enqueue (for another consumer)
		}()
	}
	ch <- 0 // main sends initial value; blocking enqueue

	wg.Wait()                    // wait for all goroutines
	fmt.Println("Count: ", <-ch) // dequeue final result
}
