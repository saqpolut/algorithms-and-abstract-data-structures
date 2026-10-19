package main

type stackNode struct {
	value string
	next  *stackNode
}

type Stack struct {
	top *stackNode
}

func (s *Stack) Push(value string) {
	s.top = &stackNode{value: value, next: s.top}
}

func (s *Stack) Pop() (string, bool) {
	if s.top == nil {
		return "", false
	}
	value := s.top.value
	s.top = s.top.next
	return value, true
}

func (s *Stack) Peek() (string, bool) {
	if s.top == nil {
		return "", false
	}
	return s.top.value, true
}

func (s *Stack) PeekTwo() (top, below string, ok bool) {
	if s.top == nil || s.top.next == nil {
		return "", "", false
	}
	return s.top.value, s.top.next.value, true
}

func (s *Stack) IsEmpty() bool {
	return s.top == nil
}
