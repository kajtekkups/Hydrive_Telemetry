
class Macierz:

    def __init__(self, matrix, tuple_elements=0):

        # create matrix with size from matrix argument
        if type(matrix) is tuple:

            self.matrix = []
            for row in range(matrix[0]):
                self.matrix.append([tuple_elements] * matrix[1])

        else:
            self.matrix = matrix

    def size(self):
        return [len(self.matrix), len(self.matrix[0])]

    def __getitem__(self, coordinates):
        if type(coordinates) is tuple:
            return self.matrix[coordinates[0]][coordinates[1]]
        else:
            return self.matrix[coordinates]

    def __add__(self, matrixToAdd):

        if self.size() == matrixToAdd.size():
            result = []
            for row in range(self.size()[0]):
                result.append([element1 + element2 for element1, element2 in zip(self.matrix[row], matrixToAdd[row])])

            return Macierz(result)
        else:
            return non

    def __mul__(self, matrixToMul):
        selfSize = self.size()
        matrixMulSize = matrixToMul.size()

        result = [[0 for i in range(selfSize[0])] for j in range(matrixMulSize[1])]
        if selfSize == list(reversed(matrixMulSize)):

            for RowVal in range(selfSize[0]):
                for ColVal in range(matrixMulSize[1]):
                    for sumNum in range(matrixMulSize[0]):
                        result[RowVal][ColVal] += self.matrix[RowVal][sumNum] * matrixToMul[sumNum][ColVal]

        return Macierz(result)

def Transpozycja( Macierz ):
    row, col = Macierz.size()
    T = [[0 for i in range( row ) ] for j in range( col )]
    for i in range(row):
        for j in range(col):
            T[j][i] = Macierz [i][j]
    return T

m1 = Macierz(
[ [1, 0, 2],
  [-1, 3, 1] ]
)

m2 = Macierz(
[ [1, 1, 1],
  [1, 1, 1]]
)

m3 = Macierz(
[ [3, 1],
  [2, 1],
  [1, 0]]
)

print(Transpozycja(m3))
print((m1 + m2).matrix)
print((m1 * m3).matrix)


