import precompile
try:
    res = precompile.run("quest/item/ring_warp.quest", "ring_warp.quest")
    print "RESULT: " + str(res)
except Exception as e:
    print "CRASH: " + str(e)
