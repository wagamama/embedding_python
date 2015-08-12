def multiply(a, b, callback=None):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    if callback is None:
        return c
    else:
        callback(c)
