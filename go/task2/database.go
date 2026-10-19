package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Database map[string]Set

func LoadDatabase(filename string) (Database, error) {
	db := make(Database)

	file, err := os.Open(filename)
	if err != nil {
		if os.IsNotExist(err) {
			return db, nil
		}
		return nil, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) < 2 || fields[0] != "E" {
			continue
		}

		name := fields[1]
		if db[name] == nil {
			db[name] = NewSet()
		}
		for _, value := range fields[2:] {
			db[name].Add(value)
		}
	}

	return db, scanner.Err()
}

func (db Database) Save(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	names := make([]string, 0, len(db))
	for name := range db {
		names = append(names, name)
	}
	// Детерминированный порядок записей.
	sortStrings(names)

	for _, name := range names {
		fmt.Fprintf(writer, "E %s", name)
		for _, value := range db[name].SortedValues() {
			fmt.Fprintf(writer, " %s", value)
		}
		fmt.Fprintln(writer)
	}

	return nil
}

func sortStrings(values []string) {
	for i := 1; i < len(values); i++ {
		for j := i; j > 0 && values[j] < values[j-1]; j-- {
			values[j], values[j-1] = values[j-1], values[j]
		}
	}
}
