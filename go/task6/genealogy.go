package main

import (
	"fmt"
	"sort"
)

type Person struct {
	name        string
	parent      int
	descendants int
}

func CountDescendants(people []Person) bool {
	for i := range people {
		ancestor := people[i].parent
		steps := 0

		for ancestor != -1 {
			if ancestor < 0 || ancestor >= len(people) || steps >= len(people) {
				return false
			}
			people[ancestor].descendants++
			ancestor = people[ancestor].parent
			steps++
		}
	}
	return true
}

func PrintGenealogy(people []Person, root int) {
	ids := make([]int, len(people))
	for i := range ids {
		ids[i] = i
	}
	sort.Slice(ids, func(i, j int) bool {
		return people[ids[i]].name < people[ids[j]].name
	})

	for _, id := range ids {
		fmt.Printf("%s %d\n", people[id].name, people[id].descendants)
	}

	fmt.Println("\nДерево родословной:")
	fmt.Printf("%s (%d потомков)\n", people[root].name, people[root].descendants)
	printChildren(people, ids, root, 1)
}

func printChildren(people []Person, ids []int, parent, depth int) {
	for _, id := range ids {
		if people[id].parent != parent {
			continue
		}
		for i := 0; i < depth; i++ {
			fmt.Print("    ")
		}
		fmt.Printf("|-- %s (%d потомков)\n",
			people[id].name, people[id].descendants)
		printChildren(people, ids, id, depth+1)
	}
}
