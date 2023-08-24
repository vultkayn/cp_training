#! /usr/env python3

dico = {}

def readF():
    global dico
    # generation of dictionary
    while line := input():
        if not line:
            break
        eng, foreign = line.split(" ")
        dico[foreign] = eng
        # yields foreign words to look for
    try:
        while to_translate := input():
            yield to_translate
    except:
        pass
        
for word in readF():
    print(dico.get(word, "eh"))