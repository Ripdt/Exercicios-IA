#include <iostream>

void print(bool m[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::cout << m[i][j] << "\t";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool isValidSpace(bool m[8][8], size_t i, size_t j) {
  for (int k = 0; k < i; k++)
    if (m[k][j]) return false;

  int ik = i - 1, jk = j - 1;
  while (ik > -1 && jk > -1)
    if (m[ik--][jk--]) return false;

  ik = i + 1, jk = j + 1;
  while (ik < 8 && jk < 8)
    if (m[ik++][jk++]) return false;

  return true;
}

bool placeQueen(bool (&m)[8][8], size_t i, size_t j) {
  if (i == 8) return true;
  if (!isValidSpace(m, i, j)) return false;

  m[i][j] = true;
  for (int k = 0; k < 8; k++)
    if (placeQueen(m, i + 1, k)) return true;

  m[i][j] = false;

  return false;
}

int main() {
	bool m[8][8] = {false};
  placeQueen(m, 0, 0);
  print(m);
	return 0;
}