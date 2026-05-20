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
	ctx context.Context, reqCh <-chan Request, instances int, fn func(*Request) *Request,
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

func server(ctx context.Context, outCh4 <-chan Request) {
	for {
		select {
		case completed := <-outCh4:
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

const (
	STAGE1_INSTANCES = 2
	STAGE2_INSTANCES = 5
	STAGE3_INSTANCES = 10
	STAGE4_INSTANCES = 5
)

// Pipelined logic without higher order functions
func main() {
	ctx, cancel := context.WithCancel(context.Background())
	reqCh := make(chan Request, 100)
	outCh1 := make(chan Request)
	outCh2 := make(chan Request)
	outCh3 := make(chan Request)
	outCh4 := make(chan Request)

	// Make pipeline of 4 stages
	// Stage 1
	for i := 0; i < STAGE1_INSTANCES; i++ {
		go func() {
			for req := range reqCh {
				select {
				case outCh1 <- *processReqStep1(&req):
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	// Stage 2
	for i := 0; i < STAGE2_INSTANCES; i++ {
		go func() {
			for req := range outCh1 {
				select {
				case outCh2 <- *processReqStep2(&req):
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	// Stage 3
	for i := 0; i < STAGE3_INSTANCES; i++ {
		go func() {
			for req := range outCh2 {
				select {
				case outCh3 <- *processReqStep3(&req):
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	// Stage 4
	for i := 0; i < STAGE4_INSTANCES; i++ {
		go func() {
			for req := range outCh3 {
				select {
				case outCh4 <- *processReqStep4(&req):
				case <-ctx.Done():
					return
				}
			}
		}()
	}
	start := make(chan struct{})
	// Spawn clients and servers
	for i := 0; i < numClients; i++ {
		go func() {
			<-start
			client(ctx, reqCh)
		}()
	}

	for i := 0; i < numServers; i++ {
		go func() {
			<-start
			server(ctx, outCh4)
		}()
	}

	time.Sleep(1 * time.Second)
	close(start)
	time.Sleep(2 * time.Second)
	cancel()

	fmt.Printf("# of completed reqs: %v\n", atomic.LoadInt64(&completedReqs))
}
