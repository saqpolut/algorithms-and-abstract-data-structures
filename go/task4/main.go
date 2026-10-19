package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)

	text, err := reader.ReadString('\n')
	if err != nil && len(text) == 0 {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать текст")
		os.Exit(1)
	}
	text = strings.TrimRight(text, "\r\n")

	pattern, err := reader.ReadString('\n')
	if err != nil && len(pattern) == 0 {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать шаблон")
		os.Exit(1)
	}
	pattern = strings.TrimRight(pattern, "\r\n")

	var input StringArray
	input.Add(text)
	input.Add(pattern)

	t, _ := input.Get(0)
	p, _ := input.Get(1)

	if Matches(t, p) {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}
