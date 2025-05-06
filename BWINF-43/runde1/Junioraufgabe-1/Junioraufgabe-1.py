# -*- coding: utf-8 -*-
"""
Created on Mon Sep  9 12:30:39 2024

@author: dodoq
"""

import math
#############################
# 1. Einlesen der Textdatei #
#############################

Liste = []
with open("eigenes Beispiel.txt", encoding = "utf-8") as f:
    for line in f:
       Liste.append(line)
for i in range(len(Liste)-1):
    Liste[i] = Liste[i][0:len(Liste[i])-1]
    Liste[i] = Liste[i].split(",")
    
########################################
# 2. Essentielle Variablen deklarieren #
########################################

Interessenten = int(Liste[0][0])
Höhe = int(Liste[1][0])
Breite = int(Liste[-1])
Minimum = Interessenten
Maximum = math.floor(1.1*Interessenten)
Abweichung = min(Höhe, Breite)/max(Höhe, Breite)
Anzahl = 1
Zeile = 1
Spalte = 1

#################################################################################################
# 3. Jede Anzahl von Kleingärten durchprobieren und die Differenz der Rechteckseiten minimieren #
#################################################################################################

for i in range(Minimum, Maximum + 1, 1):
    Teiler = []
    for j in range(1, math.floor(math.sqrt(i))+1, 1):
        if i % j == 0:
            Teiler.append([j,int(i/j)])
    for k in range(1, len(Teiler), 1):
        Quotient1 = min(Breite/Teiler[k][0], Höhe/Teiler[k][1])/max(Breite/Teiler[k][0], Höhe/Teiler[k][1])
        Quotient2 = min(Breite/Teiler[k][1], Höhe/Teiler[k][0])/max(Breite/Teiler[k][1], Höhe/Teiler[k][0])
        if Quotient1 > Abweichung:
            Abweichung = Quotient1
            Anzahl = i
            Zeile = Teiler[k][1]
            Spalte = Teiler[k][0]
        if Quotient2 > Abweichung:
            Abweichung = Quotient2
            Anzahl = i
            Zeile = Teiler[k][0]
            Spalte = Teiler[k][1]
            
##############
# 4. Ausgabe #
##############

print("Am optimalsten ist es, den Garten in", Anzahl, "viele Kleingärten zu unterteilen, mit", Spalte , 
      "pro Zeile und", Zeile, "pro Spalte.")


