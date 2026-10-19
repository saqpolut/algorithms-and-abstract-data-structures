package main

import "sort"

type Set map[string]struct{}

func NewSet() Set {
	return make(Set)
}

func (s Set) Add(value string) {
	s[value] = struct{}{}
}

func (s Set) Delete(value string) {
	delete(s, value)
}

func (s Set) Contains(value string) bool {
	_, ok := s[value]
	return ok
}

func (s Set) SortedValues() []string {
	values := make([]string, 0, len(s))
	for value := range s {
		values = append(values, value)
	}
	sort.Strings(values)
	return values
}

func Union(a, b Set) Set {
	result := NewSet()
	for value := range a {
		result.Add(value)
	}
	for value := range b {
		result.Add(value)
	}
	return result
}

func Intersection(a, b Set) Set {
	result := NewSet()
	for value := range a {
		if b.Contains(value) {
			result.Add(value)
		}
	}
	return result
}

func Difference(a, b Set) Set {
	result := NewSet()
	for value := range a {
		if !b.Contains(value) {
			result.Add(value)
		}
	}
	return result
}
