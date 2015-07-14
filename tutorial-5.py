import cmodule


def multiply(a, b):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    cmodule.callback(c)


def main():
    multiply(3, 5)

if __name__ == '__main__':
    main()
