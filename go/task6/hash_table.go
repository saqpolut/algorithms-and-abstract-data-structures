package main

import "hash/fnv"

type Entry struct {
	key   string
	value int
	used  bool
}

type HashTable struct {
	entries    []Entry
	method     string
	probes     int
	collisions int
}

func NewHashTable(expected int, method string) *HashTable {
	capacity := expected*2 + 3
	for !isPrime(capacity) {
		capacity++
	}
	return &HashTable{
		entries: make([]Entry, capacity),
		method:  method,
	}
}

func isPrime(n int) bool {
	if n < 2 {
		return false
	}
	for i := 2; i*i <= n; i++ {
		if n%i == 0 {
			return false
		}
	}
	return true
}

func hashValue(text string, seed uint32) uint64 {
	h := fnv.New64a()
	_, _ = h.Write([]byte{byte(seed), byte(seed >> 8), byte(seed >> 16), byte(seed >> 24)})
	_, _ = h.Write([]byte(text))
	return h.Sum64()
}

func (t *HashTable) startAndStep(key string) (int, int) {
	capacity := len(t.entries)
	start := int(hashValue(key, 1) % uint64(capacity))

	if t.method == "linear" {
		return start, 1
	}

	// Ёмкость таблицы простая, поэтому шаг 1..capacity-1
	// обеспечивает полный обход ячеек.
	step := 1 + int(hashValue(key, 2)%uint64(capacity-1))
	return start, step
}

func (t *HashTable) Find(key string) (int, bool) {
	index, step := t.startAndStep(key)

	for checked := 0; checked < len(t.entries); checked++ {
		t.probes++
		entry := t.entries[index]

		if !entry.used {
			return 0, false
		}
		if entry.key == key {
			return entry.value, true
		}

		t.collisions++
		index = (index + step) % len(t.entries)
	}
	return 0, false
}

func (t *HashTable) Insert(key string, value int) bool {
	index, step := t.startAndStep(key)

	for checked := 0; checked < len(t.entries); checked++ {
		t.probes++
		entry := &t.entries[index]

		if !entry.used || entry.key == key {
			entry.key = key
			entry.value = value
			entry.used = true
			return true
		}

		t.collisions++
		index = (index + step) % len(t.entries)
	}
	return false
}
