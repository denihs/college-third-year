b50 = 0
b10 = 0
b5 = 0
b1 = 0

def calc(value):
	global b50, b10, b5, b1

	if value < 0:
		return 0
	if value - 50 >= 0:
		b50 += 1
		calc(value - 50)

	elif value - 10 >= 0:
		b10 += 1
		calc(value - 10)

	elif value - 5 >= 0:
		b5 += 1
		calc(value - 5)

	elif value - 1 >= 0:
		b1 += 1
		calc(value - 1)

calc(137)
print(b50)
print(b10)
print(b5)
print(b1)
