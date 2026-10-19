package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	in := bufio.NewReader(os.Stdin)

	var capacity, queryCount int
	if _, err := fmt.Fscan(in, &capacity, &queryCount); err != nil ||
		capacity < 0 || queryCount < 0 {
		fmt.Fprintln(os.Stderr, "Ожидаются неотрицательные cap и Q")
		os.Exit(1)
	}

	cache := NewLRUCache(capacity)
	firstResult := true

	for i := 0; i < queryCount; i++ {
		var operation string
		if _, err := fmt.Fscan(in, &operation); err != nil {
			fmt.Fprintln(os.Stderr, "Ошибка чтения запроса")
			os.Exit(1)
		}

		switch operation {
		case "SET":
			var key, value int
			if _, err := fmt.Fscan(in, &key, &value); err != nil {
				fmt.Fprintln(os.Stderr, "Для SET нужны ключ и значение")
				os.Exit(1)
			}
			cache.Set(key, value)

		case "GET":
			var key int
			if _, err := fmt.Fscan(in, &key); err != nil {
				fmt.Fprintln(os.Stderr, "Для GET нужен ключ")
				os.Exit(1)
			}

			if !firstResult {
				fmt.Print(" ")
			}
			fmt.Print(cache.Get(key))
			firstResult = false

		default:
			fmt.Fprintln(os.Stderr, "Неизвестная операция:", operation)
			os.Exit(1)
		}
	}

	fmt.Println()
}
