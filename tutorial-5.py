import cmodule


def multiply(a, b):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    cmodule.callback(c)
