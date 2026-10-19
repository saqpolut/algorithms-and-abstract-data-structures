package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	var filename, query string

	for i := 1; i < len(os.Args); i++ {
		switch os.Args[i] {
		case "--file":
			if i+1 < len(os.Args) {
				i++
				filename = os.Args[i]
			}
		case "--query":
			if i+1 < len(os.Args) {
				i++
				query = os.Args[i]
			}
		}
	}

	if filename == "" || query == "" {
		fmt.Fprintf(os.Stderr, "Использование: %s --file file.data --query \"КОМАНДА\"\n", os.Args[0])
		os.Exit(1)
	}

	db, err := LoadDatabase(filename)
	if err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка чтения базы:", err)
		os.Exit(1)
	}

	fields := strings.Fields(query)
	if len(fields) == 0 {
		fmt.Fprintln(os.Stderr, "Пустой запрос")
		os.Exit(1)
	}

	changed := false

	switch fields[0] {
	case "SETADD", "SETDEL", "SET_AT":
		if len(fields) != 3 {
			fmt.Fprintln(os.Stderr, "Формат: команда имя_множества элемент")
			os.Exit(1)
		}
		name, value := fields[1], fields[2]

		switch fields[0] {
		case "SETADD":
			if db[name] == nil {
				db[name] = NewSet()
			}
			if db[name].Contains(value) {
				fmt.Println("ALREADY_EXISTS")
			} else {
				db[name].Add(value)
				fmt.Println(value)
				changed = true
			}

		case "SETDEL":
			if db[name] != nil && db[name].Contains(value) {
				db[name].Delete(value)
				fmt.Println(value)
				changed = true
			} else {
				fmt.Println("NOT_FOUND")
			}

		case "SET_AT":
			fmt.Println(db[name].Contains(value))
		}

	case "SETUNION", "SETINTERSECTION", "SETDIFF":
		if len(fields) != 4 {
			fmt.Fprintln(os.Stderr, "Формат: команда result A B")
			os.Exit(1)
		}

		resultName, aName, bName := fields[1], fields[2], fields[3]
		a, b := db[aName], db[bName]
		if a == nil || b == nil {
			fmt.Fprintln(os.Stderr, "Оба исходных множества должны существовать")
			os.Exit(1)
		}

		switch fields[0] {
		case "SETUNION":
			db[resultName] = Union(a, b)
		case "SETINTERSECTION":
			db[resultName] = Intersection(a, b)
		case "SETDIFF":
			db[resultName] = Difference(a, b)
		}

		fmt.Println(db[resultName].SortedValues())
		changed = true

	default:
		fmt.Fprintln(os.Stderr, "Неизвестная команда:", fields[0])
		os.Exit(1)
	}

	if changed {
		if err := db.Save(filename); err != nil {
			fmt.Fprintln(os.Stderr, "Ошибка сохранения базы:", err)
			os.Exit(1)
		}
	}
}
