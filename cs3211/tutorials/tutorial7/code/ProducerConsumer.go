package main

import (
	"fmt"
	"time"
)

func producer(done chan struct{}, q chan int) {
	for {
		select {
		case q <- 1:
		case <-done:
			return
		}
	}
}

func consumer(done chan struct{}, q chan int, sumCh chan<- int) {
	sum := 0
	for {
		select {
		case <-done:
			sumCh <- sum
			close(sumCh)
			return
		case num := <-q:
			sum += num
		}
	}
}

var (
	NumProducer = 5
	NumConsumer = 5
)

func main() {
	start, done := make(chan struct{}), make(chan struct{})
	q := make(chan int)

	sumChs := make([]chan int, 0, NumConsumer) // 1 channel per consumer
	for i := 0; i < NumConsumer; i++ {
		sumCh := make(chan int, 1)
		sumChs = append(sumChs, sumCh)
	}

	for i := 0; i < NumProducer; i++ {
		go func() {
			<-start
			producer(done, q)
		}()
	}
	for j := 0; j < NumConsumer; j++ {
		j := j // capture j in the scope
		go func() {
			<-start
			consumer(done, q, sumChs[j])
		}()
	}

	close(start)            // signal to all goroutines to start
	time.Sleep(time.Second) // run for 1 second
	close(done)             // signal to all goroutines they should exit

	// collect all sums
	sum := 0
	for _, ch := range sumChs { // NOTE: range over slice, not combined channel
		sum += <-ch
	}
	fmt.Println("Sum: ", sum)
}
