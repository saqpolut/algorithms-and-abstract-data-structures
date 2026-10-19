package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

func main() {
	in := bufio.NewScanner(os.Stdin)
	in.Split(bufio.ScanWords)

	var scores Stack
	total := 0

	for in.Scan() {
		op := in.Text()

		switch op {
		case "C":
			if _, ok := scores.Pop(); !ok {
				fmt.Fprintln(os.Stderr, "Ошибка: C применена к пустому стеку")
				os.Exit(1)
			}

		case "D":
			last, ok := scores.Peek()
			if !ok {
				fmt.Fprintln(os.Stderr, "Ошибка: D применена к пустому стеку")
				os.Exit(1)
			}
			value, err := strconv.Atoi(last)
			if err != nil {
				fmt.Fprintln(os.Stderr, "Некорректное значение в стеке")
				os.Exit(1)
			}
			scores.Push(strconv.Itoa(value * 2))

		case "+":
			top, below, ok := scores.PeekTwo()
			if !ok {
				fmt.Fprintln(os.Stderr, "Ошибка: для + нужны два предыдущих очка")
				os.Exit(1)
			}
			a, _ := strconv.Atoi(top)
			b, _ := strconv.Atoi(below)
			scores.Push(strconv.Itoa(a + b))

		default:
			value, err := strconv.Atoi(op)
			if err != nil || value <= 0 {
				fmt.Fprintf(os.Stderr, "Некорректная операция или число: %s\n", op)
				os.Exit(1)
			}
			scores.Push(op)
		}

		if err := in.Err(); err != nil {
			fmt.Fprintln(os.Stderr, "Ошибка чтения:", err)
			os.Exit(1)
		}
	}

	for !scores.IsEmpty() {
		value, _ := scores.Pop()
		score, _ := strconv.Atoi(value)
		total += score
	}

	fmt.Println(total)
}
