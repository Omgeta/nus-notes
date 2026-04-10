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

func busyForLoops(loops int) {
	sum := 0
	for num := range array[:loops] {
		sum += num
	}
}

type Request struct {
	id int64
}

var (
	reqId         int64 = 0
	completedReqs int64 = 0
)

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

func toPipelineStage(
	ctx context.Context,
	reqCh <-chan Request, instances int, fn func(*Request) *Request,
) chan Request {
	outCh := make(chan Request)
	for i := 0; i < instances; i++ {
		go func() {
			for req := range reqCh {
				select {
				case outCh <- *fn(&req):
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	return outCh
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
		case completed := <-reqCh:
			completed.Done()
		case <-ctx.Done():
			return
		}
	}
}

const (
	numClients = 2
	numServers = 5
)

func init() {
	populate(array)
}

func main() {
	ctx, cancel := context.WithCancel(context.Background())
	reqCh := make(chan Request, 100)
	// Make pipeline of 4 stages
	outCh := toPipelineStage(ctx, reqCh, 2, processReqStep1)
	outCh = toPipelineStage(ctx, outCh, 5, processReqStep2)
	outCh = toPipelineStage(ctx, outCh, 10, processReqStep3)
	outCh = toPipelineStage(ctx, outCh, 5, processReqStep4)
	// Spawn a clients and servers
	start := make(chan struct{})
	for i := 0; i < numClients; i++ {
		go func() {
			<-start
			client(ctx, reqCh)
		}()
	}
	for i := 0; i < numServers; i++ {
		go func() {
			<-start
			server(ctx, outCh)
		}()
	}

	time.Sleep(1 * time.Second)
	close(start)
	time.Sleep(2 * time.Second)
	cancel()

	fmt.Printf("# of completed reqs: %v\n", atomic.LoadInt64(&completedReqs))
}
