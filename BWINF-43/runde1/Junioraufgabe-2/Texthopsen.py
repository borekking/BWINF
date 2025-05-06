# -*- coding: utf-8 -*-
"""
Created on Mon Sep  9 12:55:14 2024

@author: dodoq
"""

#############################
# 1. Einlesen der Textdatei #
#############################

Text = ""
with open("hopsen5.txt", encoding = "utf-8") as f:
    for line in f:
       Text += line    
       
#########################################
# 2. Text nur aus Buchstaben generieren #
#########################################

Text1 = ""
for i in range(0,len(Text),1):
    if (ord(Text[i]) >= 65 and ord(Text[i]) <= 90) or (ord(Text[i]) >= 97 and ord(Text[i]) <= 122) or (ord(Text[i]) == ord('ß')) or (ord(Text[i]) == ord('ä')) or (ord(Text[i]) == ord('ü')) or (ord(Text[i]) == ord('ö')) or (ord(Text[i]) == ord('Ä')) or (ord(Text[i]) == ord('Ü')) or (ord(Text[i]) == ord('Ö')): 
        Text1 += Text[i]

##################################
# 3. Weitere relevante Variablen #
##################################

Liste = [["a",1],["b",2],["c",3],["d",4],["e",5],["f",6],["g",7],["h",8],["i",9],["j",10],["k",11],["l",12],["m",13],["n",14],["o",15],["p",16],["q",17],["r",18],["s",19],["t",20],["u",21],["v",22],["w",23],["x",24],["y",25],["z",26],["ä",27],["ö",28],["ü",29],["ß",30],["A",1],["B",2],["C",3],["D",4],["E",5],["F",6],["G",7],["H",8],["I",9],["J",10],["K",11],["L",12],["M",13],["N",14],["O",15],["P",16],["Q",17],["R",18],["S",19],["T",20],["U",21],["V",22],["W",23],["X",24],["Y",25],["Z",26],["Ä",27],["Ö",28],["Ü",29]]

Textlänge = len(Text1)

counter1 = 0
position1 = 0

############################
# 4. Anzahl Sprünge zählen #
############################

while position1 < len(Text1):
    sprungweite1 = 0
    counter1 += 1
    for i in range(0,len(Liste)-1,1):
        if Liste[i][0] == Text1[position1]:
            sprungweite1 = Liste[i][1]
            break
    position1 += sprungweite1

counter2 = 0
position2 = 1

while position2 < len(Text1):
    sprungweite2 = 0
    counter2 += 1
    for i in range(0,len(Liste)-1,1):
        if Liste[i][0] == Text1[position2]:
            sprungweite2 = Liste[i][1]
            break
    position2 += sprungweite2
    
##############
# 5. Ausgabe #
##############
    
if counter1 <= counter2:
    print("Bela gewinnt, da sie " + str(counter1) + " viele Sprünge braucht und Amira " + str(counter2) + " viele Sprünge braucht.")
else:
    print("Amira gewinnt, da sie " + str(counter2) + " viele Sprünge braucht und Bela " + str(counter1) + " viele Sprünge braucht.")
