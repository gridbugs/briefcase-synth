for i in range(0, 61):
    v12bit = int((i * 4095) / (5 * 12))
    v = (v12bit * 5) / 4095
    s = v * 12
    print(i, v12bit, v, s)
