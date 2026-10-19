package main

type Node struct {
	key, value int
	prev, next *Node
	bucketNext *Node
}

type LRUCache struct {
	capacity int
	size     int
	buckets  []*Node
	head     *Node
	tail     *Node
}

func NewLRUCache(capacity int) *LRUCache {
	bucketCount := capacity*2 + 17
	if bucketCount < 17 {
		bucketCount = 17
	}

	cache := &LRUCache{
		capacity: capacity,
		buckets:  make([]*Node, bucketCount),
		head:     &Node{},
		tail:     &Node{},
	}
	cache.head.next = cache.tail
	cache.tail.prev = cache.head
	return cache
}

func (c *LRUCache) bucket(key int) int {
	if key < 0 {
		key = -key
	}
	return key % len(c.buckets)
}

func (c *LRUCache) find(key int) *Node {
	for node := c.buckets[c.bucket(key)]; node != nil; node = node.bucketNext {
		if node.key == key {
			return node
		}
	}
	return nil
}

func (c *LRUCache) addToHash(node *Node) {
	index := c.bucket(node.key)
	node.bucketNext = c.buckets[index]
	c.buckets[index] = node
}

func (c *LRUCache) removeFromHash(key int) {
	index := c.bucket(key)
	var previous *Node

	for node := c.buckets[index]; node != nil; node = node.bucketNext {
		if node.key == key {
			if previous == nil {
				c.buckets[index] = node.bucketNext
			} else {
				previous.bucketNext = node.bucketNext
			}
			return
		}
		previous = node
	}
}

func (c *LRUCache) removeFromList(node *Node) {
	node.prev.next = node.next
	node.next.prev = node.prev
}

func (c *LRUCache) addToFront(node *Node) {
	node.prev = c.head
	node.next = c.head.next
	c.head.next.prev = node
	c.head.next = node
}

func (c *LRUCache) moveToFront(node *Node) {
	c.removeFromList(node)
	c.addToFront(node)
}

func (c *LRUCache) Get(key int) int {
	node := c.find(key)
	if node == nil {
		return -1
	}
	c.moveToFront(node)
	return node.value
}

func (c *LRUCache) Set(key, value int) {
	if node := c.find(key); node != nil {
		node.value = value
		c.moveToFront(node)
		return
	}

	if c.capacity <= 0 {
		return
	}

	node := &Node{key: key, value: value}
	c.addToFront(node)
	c.addToHash(node)
	c.size++

	if c.size > c.capacity {
		oldest := c.tail.prev
		c.removeFromList(oldest)
		c.removeFromHash(oldest.key)
		c.size--
	}
}
