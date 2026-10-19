package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	method := "double"
	showStats := false

	for _, arg := range os.Args[1:] {
		if arg == "linear" {
			method = "linear"
		}
		if arg == "--stats" {
			showStats = true
		}
	}

	in := bufio.NewReader(os.Stdin)
	var n int
	if _, err := fmt.Fscan(in, &n); err != nil || n <= 0 {
		fmt.Fprintln(os.Stderr, "N должно быть положительным")
		os.Exit(1)
	}

	people := make([]Person, n)
	ids := make(map[string]int, n)
	table := NewHashTable(n, method)
	count := 0

	getOrAdd := func(name string) (int, bool) {
		if id, ok := table.Find(name); ok {
			return id, true
		}
		if count >= n {
			return 0, false
		}
		id := count
		count++
		people[id] = Person{name: name, parent: -1}
		ids[name] = id
		if !table.Insert(name, id) {
			return 0, false
		}
		return id, true
	}

	for i := 0; i < n-1; i++ {
		var childName, parentName string
		if _, err := fmt.Fscan(in, &childName, &parentName); err != nil {
			fmt.Fprintln(os.Stderr, "Ошибка чтения пары потомок-родитель")
			os.Exit(1)
		}

		child, ok1 := getOrAdd(childName)
		parent, ok2 := getOrAdd(parentName)
		if !ok1 || !ok2 {
			fmt.Fprintln(os.Stderr, "Ошибка добавления человека")
			os.Exit(1)
		}

		if people[child].parent != -1 && people[child].parent != parent {
			fmt.Fprintln(os.Stderr, "У человека указано несколько родителей")
			os.Exit(1)
		}
		people[child].parent = parent
	}

	if count != n {
		fmt.Fprintf(os.Stderr, "Найдено людей: %d, ожидалось: %d\n", count, n)
		os.Exit(1)
	}

	root, rootCount := -1, 0
	for i := range people {
		if people[i].parent == -1 {
			root = i
			rootCount++
		}
	}
	if rootCount != 1 {
		fmt.Fprintln(os.Stderr, "В родословной должен быть ровно один родоначальник")
		os.Exit(1)
	}

	if !CountDescendants(people) {
		fmt.Fprintln(os.Stderr, "В родословной обнаружен цикл")
		os.Exit(1)
	}

	PrintGenealogy(people, root)

	if showStats {
		fmt.Fprintf(os.Stderr,
			"\nМетод: %s\nПроверок ячеек: %d\nКоллизий: %d\n",
			method, table.probes, table.collisions)
	}

	_ = strings.Builder{} // пакет strings в этой программе не требуется для алгоритма
	_ = ids
}
