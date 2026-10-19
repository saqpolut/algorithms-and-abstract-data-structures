package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strings"
)

func main() {
	reader := bufio.NewReader(os.Stdin)

	fmt.Print("Введите исходную строку: ")
	text, err := reader.ReadString('\n')
	if err != nil && len(text) == 0 {
		fmt.Fprintln(os.Stderr, "Не удалось прочитать строку")
		os.Exit(1)
	}
	text = strings.TrimRight(text, "\r\n")

	if text == "" {
		fmt.Fprintln(os.Stderr, "Исходная строка не должна быть пустой")
		os.Exit(1)
	}

	root := BuildTree(text)
	codes := BuildCodes(root)

	fmt.Println("Таблица кодов:")
	symbols := make([]int, 0, len(codes))
	for symbol := range codes {
		symbols = append(symbols, int(symbol))
	}
	sort.Ints(symbols)
	for _, symbol := range symbols {
		fmt.Printf("%q : %s\n", byte(symbol), codes[byte(symbol)])
	}

	encoded := Encode(text, codes)
	fmt.Println("Код строки:", encoded)

	fmt.Print("Введите двоичный код для декодирования: ")
	bits, _ := reader.ReadString('\n')
	bits = strings.TrimSpace(bits)

	if bits != "" {
		decoded, ok := Decode(bits, root)
		if !ok {
			fmt.Println("Некорректный или неполный код")
		} else {
			fmt.Println("Расшифрованная строка:", decoded)
		}
	}
}
