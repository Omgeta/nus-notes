package main

import (
	"context"
	"fmt"
	"time"
)

type queue chan int

func (q queue) try_enqueue(num int) bool {
	select {
	case q <- num:
		return true
	default:
	}
	return false
}

func (q queue) try_dequeue() (int, bool) {
	select {
	case num, ok := <-q:
		return num, ok
	default:
	}
	return 0, false
}

func producer(ctx context.Context, q queue) {
	for {
		select {
		case q <- 1: // normal enqueue to q with type alias to chan int
		case <-ctx.Done():
			return
		}
	}
}

func consumer(ctx context.Context, q queue, sumCh chan<- int) {
	sum := 0
	for {
		select {
		case <-ctx.Done():
			sumCh <- sum
			close(sumCh)
			return
		case num := <-q:
			sum += num
		}
	}
}

const (
	NumProducer = 5
	NumConsumer = 5
)

func main() {
	// cancel (2nd return value) can be used to manually cancel() before the timeout
	ctx, _ := context.WithTimeout(context.Background(), time.Second)

	q := make(chan int)
	sumChs := make([]chan int, 0, NumConsumer)
	for i := 0; i < NumConsumer; i++ {
		sumCh := make(chan int)
		sumChs = append(sumChs, sumCh)
	}

	for i := 0; i < NumProducer; i++ {
		go func() {
			producer(ctx, q)
		}()
	}
	for j := 0; j < NumConsumer; j++ {
		j := j
		go func() {
			consumer(ctx, q, sumChs[j])
		}()
	}

	// context.WithTimeout does the following
	/*
	   time.Sleep(time.Second)
	   cancel()
	*/

	sum := 0
	for _, ch := range sumChs {
		sum += <-ch
	}
	fmt.Println("Sum: ", sum)
}
