import math

def T(n):
	if (n < 2):
		return 1
	return 2 * T(math.floor(n/2)) + n

print (T(int(input("> "))))
