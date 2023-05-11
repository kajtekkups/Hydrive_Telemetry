def wyznacznik_2x2(Macierz):
    return Macierz[0][0] * Macierz [1][1] - Macierz[0][1] * Macierz[1][0]

def Chio( Macierz, mnoznik = 1 ):
    if len(Macierz) != len(Macierz[0]):
        return None

    if len(Macierz) == 2:
        return wyznacznik_2x2(Macierz) * mnoznik

    if not Macierz[0][0]:
        for row in range(len(Macierz)):
            if Macierz[row][0]:
                Macierz[0], Macierz[row] = Macierz[row], Macierz[0]

    Macierz2 = [[0 for i in range(len(Macierz[0]) - 1 ) ] for j in range( len(Macierz) - 1)]
    mnoznik *= 1 / (Macierz[0][0] ** (len(Macierz) - 2) )

    for row in range(len(Macierz) - 1 ):
        for col in range(len(Macierz) - 1 ):
            macierzPomocnicza = [[0 for i in range(2) ] for j in range(2)]
            macierzPomocnicza[0][0] = Macierz[0][0]
            macierzPomocnicza[0][1] = Macierz[0][col + 1]
            macierzPomocnicza[1][0] = Macierz[row + 1][0]
            macierzPomocnicza[1][1] = Macierz[row + 1][col + 1]
            Macierz2[row][col] = wyznacznik_2x2(macierzPomocnicza)
    return Chio(Macierz2, mnoznik)

macierz =   [[0 , 1 , 1 , 2 , 3],
            [4 , 2 , 1 , 7 , 3],
            [2 , 1 , 2 , 4 , 7],
            [9 , 1 , 0 , 7 , 0],
            [1 , 4 , 7 , 2 , 2]]

wyznacznik = Chio(macierz)

print(wyznacznik)