package main

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// Representing each individual event
type Event struct {
	id       int64
	procTime time.Duration
}

type EventFunc func(Event) Event

type worker struct {
	inputCh  <-chan Event
	outputCh chan<- Event
}

func newWorker(inputCh, outputCh chan Event) *worker {
	return &worker{
		inputCh:  inputCh,
		outputCh: outputCh,
	}
}

func (w *worker) start(
	ctx context.Context,
	fn EventFunc, wg *sync.WaitGroup,
) {
	go func() {
		defer wg.Done()
		for {
			select {
			case e, more := <-w.inputCh:
				if !more {
					return
				}
				select {
				case w.outputCh <- fn(e):
				case <-ctx.Done():
					return
				}
			case <-ctx.Done():
				return
			}
		}
	}()
}

// Randomly generates events and pushes them into an output channel `dataCh`
func genEventsCh() chan Event {
	dataCh := make(chan Event)
	go func() {
		counter := int64(1)
		rand.Seed(time.Now().Unix())
		for i := 0; i < 30; i++ {
			dataCh <- Event{
				id:       counter,
				procTime: time.Duration(rand.Intn(100)) * time.Millisecond,
			}
			counter++
		}
		close(dataCh)
	}()
	return dataCh
}

func main() {
	ctx, cancel := context.WithCancel(context.Background())

	// Create the worker's input (unprocessedCh)
	// and the output (processedCh) channels
	processedCh := make(chan Event, 1)
	unprocessedCh := genEventsCh()

	// Fan-out the stream of input to multiple workers
	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		newWorker(unprocessedCh, processedCh).
			start(
				ctx,
				func(e Event) Event {
					time.Sleep(e.procTime)
					return e
				},
				&wg,
			)
	}
	readerDone := make(chan struct{})
	go func() {
		for output := range processedCh {
			fmt.Printf("Event id: %d\n", output.id)
		}
		close(readerDone)
	}()
	wg.Wait()

	// Close processedCh and wait for reader to finish reading
	close(processedCh)
	<-readerDone
	// Cleanup any dangling goroutines (there should not be any)
	cancel()
}
