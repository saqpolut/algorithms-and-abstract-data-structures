package main

func Matches(text, pattern string) bool {
	// dp[j] после обработки очередного символа текста означает,
	// совпадают ли уже обработанные части текста и шаблона до j.
	dp := make([]bool, len(pattern)+1)
	dp[0] = true

	for j := 1; j <= len(pattern); j++ {
		if pattern[j-1] == '*' {
			dp[j] = dp[j-1]
		}
	}

	for i := 1; i <= len(text); i++ {
		next := make([]bool, len(pattern)+1)

		for j := 1; j <= len(pattern); j++ {
			switch pattern[j-1] {
			case '*':
				// Пустая последовательность или поглощение символа.
				next[j] = next[j-1] || dp[j]
			case '?':
				next[j] = dp[j-1]
			default:
				next[j] = dp[j-1] && text[i-1] == pattern[j-1]
			}
		}

		dp = next
	}

	return dp[len(pattern)]
}
