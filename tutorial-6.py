def multiply(a, b, callback):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    callback(c)

def main():
    def callback(c):
        print "result:", c
    multiply(3, 5, callback)

if __name__ == '__main__':
    main()
