def multiply(a, b, callback):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    callback(c)
