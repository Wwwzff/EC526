for i in range(13, 16):
    for s in range(2):
        curPow = 2**i
        outFile = open('./tests/test'+str(curPow)+'_'+str(s)+'.txt', 'w')
        outFile.write(str(curPow)+" "+str(curPow)+"\n")

        for a in range(curPow):
            for b in range(curPow):
                for rgb in range(3):
                    outFile.write("0 ")
            outFile.write("\n")

        outFile.close()
