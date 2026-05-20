package main

import (
	"context"
	"fmt"
	"math/rand"
	"sync/atomic"
	"time"
)

var array = make([]int, 10000000, 10000000)

func populate(array []int) {
	rand.Seed(time.Now().Unix())
	for i := 0; i < len(array); i++ {
		array[i] = rand.Int()
	}
}

func init() {
	populate(array)
}

func busyForLoops(loops int) {
	sum := 0
	for num := range array[:loops] {
		sum += num
	}
}

type Request struct {
	id int64
}

func NewReq() *Request {
	for {
		id := atomic.LoadInt64(&reqId)
		if atomic.CompareAndSwapInt64(&reqId, id, id+1) {
			return &Request{id + 1}
		}
	}
}

func (r *Request) Done() {
	atomic.AddInt64(&completedReqs, 1)
	fmt.Printf("req %v\n", r.id)
}

var (
	reqId         int64 = 0
	completedReqs int64 = 0
)

func processReqStep1(req *Request) *Request {
	busyForLoops(2000000)
	return req
}

func processReqStep2(req *Request) *Request {
	busyForLoops(5000000)
	return req
}

func processReqStep3(req *Request) *Request {
	busyForLoops(10000000)
	return req
}

func processReqStep4(req *Request) *Request {
	busyForLoops(5000000)
	return req
}

func client(ctx context.Context, reqCh chan<- Request) {
	for {
		select {
		case reqCh <- *NewReq():
		case <-ctx.Done():
			return
		}
	}
}

func server(ctx context.Context, reqCh <-chan Request) {
	for {
		select {
		case req := <-reqCh:
			processReqStep1(&req)
			processReqStep2(&req)
			processReqStep3(&req)
			processReqStep4(&req)
			req.Done()
		case <-ctx.Done():
			return
		}
	}
}

const (
	numClients = 2
	numServers = 5
)

func main() {
	reqCh := make(chan Request, 100)
	ctx, cancel := context.WithCancel(context.Background())

	start := make(chan struct{})

	// Spawn servers and clients
	for i := 0; i < numClients; i++ {
		go func() {
			<-start
			client(ctx, reqCh)
		}()
	}
	for i := 0; i < numServers; i++ {
		go func() {
			<-start
			server(ctx, reqCh)
		}()
	}

	close(start)
	time.Sleep(2 * time.Second)
	cancel() // kill all routines

	fmt.Printf("# of completed reqs: %v\n", atomic.LoadInt64(&completedReqs))
}
