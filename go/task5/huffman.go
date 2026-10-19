package main

import "strings"

type Node struct {
	symbol      byte
	frequency   int
	left, right *Node
}

func BuildTree(text string) *Node {
	frequencies := make(map[byte]int)
	for i := 0; i < len(text); i++ {
		frequencies[text[i]]++
	}

	var nodes []*Node
	for symbol, frequency := range frequencies {
		nodes = append(nodes, &Node{symbol: symbol, frequency: frequency})
	}
	if len(nodes) == 0 {
		return nil
	}

	for len(nodes) > 1 {
		first, second := 0, 1
		if nodes[second].frequency < nodes[first].frequency {
			first, second = second, first
		}

		for i := 2; i < len(nodes); i++ {
			if nodes[i].frequency < nodes[first].frequency {
				second = first
				first = i
			} else if nodes[i].frequency < nodes[second].frequency {
				second = i
			}
		}

		left, right := nodes[first], nodes[second]
		if first > second {
			first, second = second, first
		}
		nodes = append(nodes[:second], nodes[second+1:]...)
		nodes = append(nodes[:first], nodes[first+1:]...)

		nodes = append(nodes, &Node{
			frequency: left.frequency + right.frequency,
			left:      left,
			right:     right,
		})
	}

	return nodes[0]
}

func BuildCodes(root *Node) map[byte]string {
	codes := make(map[byte]string)

	var walk func(*Node, string)
	walk = func(node *Node, code string) {
		if node == nil {
			return
		}
		if node.left == nil && node.right == nil {
			if code == "" {
				code = "0"
			}
			codes[node.symbol] = code
			return
		}
		walk(node.left, code+"0")
		walk(node.right, code+"1")
	}

	walk(root, "")
	return codes
}

func Encode(text string, codes map[byte]string) string {
	var result strings.Builder
	for i := 0; i < len(text); i++ {
		result.WriteString(codes[text[i]])
	}
	return result.String()
}

func Decode(bits string, root *Node) (string, bool) {
	if root == nil || bits == "" {
		return "", false
	}

	if root.left == nil && root.right == nil {
		for i := 0; i < len(bits); i++ {
			if bits[i] != '0' {
				return "", false
			}
		}
		return strings.Repeat(string([]byte{root.symbol}), len(bits)), true
	}

	var result strings.Builder
	current := root

	for i := 0; i < len(bits); i++ {
		switch bits[i] {
		case '0':
			current = current.left
		case '1':
			current = current.right
		default:
			return "", false
		}

		if current == nil {
			return "", false
		}

		if current.left == nil && current.right == nil {
			result.WriteByte(current.symbol)
			current = root
		}
	}

	if current != root {
		return "", false
	}
	return result.String(), true
}
