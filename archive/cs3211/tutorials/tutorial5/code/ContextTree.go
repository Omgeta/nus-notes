package main

import (
	"context"
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"
)

// Returns a `sigs` channel that blocks until either the program sees either signal
func handleSigs() chan struct{} {
	done := make(chan struct{})
	go func() {
		sigs := make(chan os.Signal, 1)
		signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
		<-sigs
		close(done)
	}()
	return done
}

var (
	timeout1 = 4 * time.Second
	timeout2 = 2 * time.Second
)

func main() {
	startTime := time.Now()

	ctx, _ := context.WithTimeout(context.Background(), timeout1)

	ctx2, _ := context.WithTimeout(ctx, timeout2)
	go func() {
		<-ctx2.Done()
		fmt.Printf("ctx2 done at %v\n", time.Now().Sub(startTime))
	}()

	ctx3, cancel := context.WithCancel(ctx)
	go func() {
		<-ctx3.Done()
		fmt.Printf("ctx3 done at %v\n", time.Now().Sub(startTime))
	}()

	go func() {
		<-handleSigs()
		cancel()
		fmt.Printf("signal in at %v\n", time.Now().Sub(startTime))
	}()

	<-ctx.Done()

	fmt.Printf("ctx done at %v\n", time.Now().Sub(startTime))
}
