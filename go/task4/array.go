package main

type StringArray struct {
	data []string
}

func (a *StringArray) Add(value string) {
	a.data = append(a.data, value)
}

func (a *StringArray) Get(index int) (string, bool) {
	if index < 0 || index >= len(a.data) {
		return "", false
	}
	return a.data[index], true
}
