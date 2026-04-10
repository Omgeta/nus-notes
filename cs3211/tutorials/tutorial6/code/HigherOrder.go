package main

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type Event struct {
	id       int64
	procTime time.Duration
}

type EventFunc func(Event) Event

type worker struct {
	inputCh  chan Event
	outputCh chan Event
}

func newWorker() *worker {
	return &worker{
		inputCh:  make(chan Event),
		outputCh: make(chan Event),
	}
}

func (w *worker) start(
	ctx context.Context,
	fn EventFunc, workerQueue chan *worker, wg *sync.WaitGroup,
) {
	go func() {
		defer func() {
			close(w.outputCh)
			wg.Done()
		}()
		for {
			select {
			case workerQueue <- w: // <-- change: sign up for work
				e := <-w.inputCh
				w.outputCh <- fn(e)
			case <-ctx.Done():
				return
			}
		}
	}()
}

func orderedMux(
	ctx context.Context, cancel context.CancelFunc,
	inputCh chan Event, workerQueue chan *worker, workerOutputCh chan chan Event,
) {
	go func() {
		for {
			select {
			case e, more := <-inputCh:
				if !more { // genEventsCh is finished
					cancel() // signal to workers to terminate
					return
				}
				select {
				case w := <-workerQueue:
					workerOutputCh <- w.outputCh
					w.inputCh <- e
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
	numWorkers := 10
	ctx, cancel := context.WithCancel(context.Background())
	workerQueue := make(chan *worker)
	workerOutputCh := make(chan chan Event, numWorkers)
	inputCh := genEventsCh()
	orderedMux(ctx, cancel, inputCh, workerQueue, workerOutputCh)
	var wg sync.WaitGroup
	for i := 0; i < numWorkers; i++ {
		wg.Add(1)
		newWorker().start(ctx, func(e Event) Event {
			time.Sleep(e.procTime)
			return e
		}, workerQueue, &wg)
	}

	readerDone := make(chan struct{})
	go func() {
		for outputCh := range workerOutputCh { // <-- reading a stream of promises
			output, more := <-outputCh // <-- read from the promise once only
			if !more {
				break // <-- a worker breaks its promise; workers exiting
			}
			fmt.Printf("Event id: %d\n", output.id)
		}
		close(readerDone)
	}()
	// stop all workers and wait for them to exit
	wg.Wait()

	close(workerOutputCh)
	<-readerDone
	cancel()
}
