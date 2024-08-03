#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "lab8part2.h"
#include "liblab8part2.h"

void printBoard(char board[][26], int n);
void fillBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol);
bool checkValidMove(char board[][26], int n, char colour, int row, int col);
void flipPiece(char board[][26], int n, char colour, int row, int col);
void decideTurn(char board[][26], int n, char compColour);
bool humanMove(char board[][26], int n, char humanColour, char compColour);
void compMove(char board[][26], int n, char compColour, char humanColour);
bool checkAllMoves(char board[][26], int n, char colour);
void printWinner(char board[][26], int n);
bool checkEmptyBoard(char board[][26], int n);
int makeMove(const char board[][26], int n, char turn, int *row, int *col);
int minimax(char board[][26], int depth, bool maxPlayer, char turn);
void copyBoard(char board[][26], char tempBoard[][26], int n);
int countScore(char tempBoard[][26]);

int main(void) {
  int dimension;
  char computerColour;
  char board[26][26];
  printf("Enter the board dimension: ");
  scanf("%d", &dimension);
  printf("Computer plays (B/W): ");
  scanf(" %c", &computerColour);
  fillBoard(board, dimension);
  printBoard(board, dimension);
  decideTurn(board, dimension, computerColour);
  return 0;
}

void printBoard(char board[][26], int n) {
  for (int row = 0; row < n + 1; row++) {
    for (int col = 0; col < n + 1; col++) {
      if (row == 0 && col > 0) {
        printf("%c", 97 + col - 1);
      } else if (col == 0 && row > 0) {
        printf("%c ", 97 + row - 1);
      } else if (row == 0 && col == 0) {
        printf("  ");
      } else {
        printf("%c", board[row - 1][col - 1]);
      }
    }
    printf("\n");
  }
}

void fillBoard(char board[][26], int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if ((row + 1 == n / 2 && col + 1 == n / 2) ||
          (row + 1 == n / 2 + 1 && col + 1 == n / 2 + 1)) {
        board[row][col] = 'W';
      } else if ((row + 1 == n / 2 + 1 && col + 1 == n / 2) ||
                 (row + 1 == n / 2 && col + 1 == n / 2 + 1)) {
        board[row][col] = 'B';
      } else {
        board[row][col] = 'U';
      }
    }
  }
}

bool positionInBounds(int n, int row, int col) {
  bool legal = true;
  if (row >= n || col >= n || row < 0 || col < 0) {
    legal = false;
  }
  return legal;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  int checkRow = row + deltaRow;
  int checkCol = col + deltaCol;
  bool inBounds = positionInBounds(n, checkRow, checkCol);
  bool legal = false, nextLegal = false;
  if ((board[checkRow][checkCol] != colour) &&
      (board[checkRow][checkCol] != 'U') && (inBounds == true)) {
    legal = true;
  }
  inBounds = positionInBounds(n, checkRow + deltaRow, checkCol + deltaCol);
  for (int i = 1; (inBounds == true) && (nextLegal == false) &&
                  (legal == true) && (board[checkRow][checkCol] != colour) &&
                  (board[checkRow][checkCol] != 'U');
       i++) {
    checkRow = row + i * deltaRow;
    checkCol = col + i * deltaCol;
    inBounds = positionInBounds(n, checkRow, checkCol);
    inBounds = positionInBounds(n, checkRow + deltaRow, checkCol + deltaCol);
    if ((board[checkRow][checkCol] == colour) ||
        (board[checkRow][checkCol] == 'U') || (inBounds == false)) {
      legal = false;
    }
    if ((legal == true) &&
        (board[checkRow + deltaRow][checkCol + deltaCol] == colour) &&
        (inBounds == true)) {
      nextLegal = true;
    }
  }
  return nextLegal;
}

bool checkValidMove(char board[][26], int n, char colour, int row, int col) {
  for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
    for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
      bool valid =
          checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol);
      if (valid) {
        return true;
      }
    }
  }
  return false;
}

void flipPiece(char board[][26], int n, char colour, int row, int col) {
  bool inBounds;
  for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
    for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
      bool valid =
          checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol);
      if (valid == true) {
        int i = 1;
        inBounds = positionInBounds(n, row + deltaRow * i, col + deltaCol * i);
        while ((board[row + deltaRow * i][col + deltaCol * i] != colour) &&
               (board[row + deltaRow * i][col + deltaCol * i] != 'U') &&
               (valid == true) && (inBounds == true)) {
          board[row + deltaRow * i][col + deltaCol * i] = colour;
          i++;
          inBounds =
              positionInBounds(n, row + deltaRow * i, col + deltaCol * i);
        }
      }
    }
  }
}
void decideTurn(char board[][26], int n, char compColour) {
  int counter = 0;
  char humanColour;
  bool availMove = false, stop = false, stopHuman = false;
  bool noHumanMove = false, noCompMove = false;
  bool emptySpace;
  if (compColour == 'W') {  // human makes first move
    humanColour = 'B';
    while (stop == false) {
      emptySpace = checkEmptyBoard(board, n);
      if (emptySpace) {
        while (counter % 2 == 0) {  // human makes move
          counter++;
          noHumanMove = false;
          availMove = checkAllMoves(board, n, humanColour);
          if (availMove) {
            stopHuman = humanMove(board, n, humanColour, compColour);
            noHumanMove = false;
            if (stopHuman) {  // end game
              stop = true;
              return;
            }
          } else {
            noHumanMove = true;
            printf("B player has no valid move.\n");
          }
          if (noHumanMove && noCompMove) {
            printWinner(board, n);
            stop = true;
            return;
          }
        }
      } else {
        printWinner(board, n);
        stop = true;
        return;
      }
      emptySpace = checkEmptyBoard(board, n);
      if (emptySpace) {
        while (counter % 2 == 1) {  // computer makes move
          counter++;
          noCompMove = false;
          availMove = checkAllMoves(board, n, compColour);
          if (availMove) {
            compMove(board, n, compColour, humanColour);
            noCompMove = false;
          } else {
            noCompMove = true;
            printf("W player has no valid move.\n");
          }
          if (noHumanMove && noCompMove) {
            printWinner(board, n);
            stop = true;
            return;
          }
        }
      } else {
        printWinner(board, n);
        stop = true;
        return;
      }
    }
  }
  if (compColour == 'B') {  // human makes second move
    humanColour = 'W';
    while (stop == false) {
      emptySpace = checkEmptyBoard(board, n);
      if (emptySpace) {
        while (counter % 2 == 1) {  // human makes move
          counter++;
          noHumanMove = false;
          availMove = checkAllMoves(board, n, humanColour);
          if (availMove) {
            stopHuman = humanMove(board, n, humanColour, compColour);
            if (stopHuman) {  // end game
              stop = true;
              return;
            }
          } else {
            noHumanMove = true;
            printf("W player has no valid move.\n");
          }
          if (noHumanMove && noCompMove) {
            printWinner(board, n);
            stop = true;
            return;
          }
        }
      } else {
        printWinner(board, n);
        stop = true;
        return;
      }
      emptySpace = checkEmptyBoard(board, n);
      if (emptySpace) {
        while (counter % 2 == 0) {  // computer makes move
          counter++;
          noCompMove = false;
          availMove = checkAllMoves(board, n, compColour);
          if (availMove) {
            compMove(board, n, compColour, humanColour);
          } else {
            noCompMove = true;
            printf("B player has no valid move.\n");
          }
          if (noHumanMove && noCompMove) {
            printWinner(board, n);
            stop = true;
            return;
          }
        }
      } else {
        printWinner(board, n);
        stop = true;
        return;
      }
    }
  }
}
bool humanMove(char board[][26], int n, char humanColour, char compColour) {
  int row, col;
  char rowIndex, colIndex;
  bool validMove = false;
  printf ("Enter move for colour %c (RowCol): ", humanColour);
  scanf (" %c%c", &rowIndex, &colIndex);
  row = rowIndex - 97;
  col = colIndex - 97;
  validMove = checkValidMove(board, n, humanColour, row, col);
  if (validMove) {
    board[row][col] = humanColour;
    flipPiece(board, n, humanColour, row, col);
    printBoard(board, n);
    return false;
  } else {
    printf("Invalid move.\n");
    printWinner(board, n);
    return true;
  }
}

bool checkAllMoves(char board[][26], int n, char colour) {
  bool availMove = false;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        availMove = checkValidMove(board, n, colour, row, col);
        if (availMove) {
          return true;
        }
      }
    }
  }
  return false;
}

void compMove(char board[][26], int n, char compColour, char humanColour) {
  int bestRow = 0;
  int bestCol = 0;
  int mostFlipped = 0;
  int counter = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
          for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
            if (!(deltaRow == 0 && deltaCol == 0)) {
              int checkRow = row + deltaRow;
              int checkCol = col + deltaCol;
              bool inBounds = positionInBounds(n, checkRow, checkCol);
              bool legal = false, nextLegal = false;
              if ((board[checkRow][checkCol] == humanColour) &&
                  (inBounds == true)) {
                legal = true;
              }
              inBounds =
                  positionInBounds(n, checkRow + deltaRow, checkCol + deltaCol);
              for (int i = 1; (inBounds == true) && (nextLegal == false) &&
                              (legal == true) &&
                              (board[checkRow][checkCol] == humanColour);
                   i++) {
                checkRow = row + i * deltaRow;
                checkCol = col + i * deltaCol;
                inBounds = positionInBounds(n, checkRow, checkCol);
                if ((board[checkRow][checkCol] == compColour) ||
                    (board[checkRow][checkCol] == 'U') || (inBounds == false)) {
                  legal = false;
                }
                if ((legal == true) &&
                    (board[checkRow + deltaRow][checkCol + deltaCol] ==
                     compColour) &&
                    (inBounds == true)) {
                  nextLegal = checkLegalInDirection(
                      board, n, row, col, compColour, deltaRow, deltaCol);
                }
                if (nextLegal == true) {
                  counter += i;
                }
              }
            }
          }
        }
        if (counter > mostFlipped) {
          bestRow = row;
          bestCol = col;
          mostFlipped = counter;
          counter = 0;
        } else {
          counter = 0;
        }
      }
    }
  }
  flipPiece(board, n, compColour, bestRow, bestCol);
  board[bestRow][bestCol] = compColour;
  printf("Computer places %c at %c%c.", compColour, bestRow + 97, bestCol + 97);
  printBoard(board, n);
}

void printWinner(char board[][26], int n) {
  int whitePieces = 0, blackPieces = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'W') {
        whitePieces++;
      } else if (board[row][col] == 'B') {
        blackPieces++;
      }
    }
  }
  if (whitePieces > blackPieces) {
    printf("W player wins.\n");
  } else if (blackPieces > whitePieces) {
    printf("B player wins.\n");
  } else {
    printf("Draw!\n");
  }
}

bool checkEmptyBoard(char board[][26], int n) {
  bool emptySpace = false;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        emptySpace = true;
      }
    }
  }
  return emptySpace;
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  int maxEval = INT_MIN;
  int posEval;
  bool valid = false;
  char oppColour;
  if (turn == 'B') {
    oppColour = 'W';
  } else {
    oppColour = 'B';
  }
  for (int rowIndex = 0; rowIndex < n; rowIndex++) {
    for (int colIndex = 0; colIndex < n; colIndex++) {
      valid = checkValidMove(board, n, turn, rowIndex, colIndex);
      if (valid) {
        char tempBoard[26][26];
        copyBoard(board, tempBoard, n);
        flipPiece(tempBoard, n, turn, rowIndex, colIndex);
        if (turn == 'B') {
          posEval = minimax(tempBoard, 5, false, oppColour);
        } else {
          posEval = minimax(tempBoard, 5, true, oppColour);
        }
        if (posEval > maxEval) {
          maxEval = posEval;
          *row = rowIndex;
          *col = colIndex;
        }
      }
    }
  }
  return 0;
}

int minimax(char board[][26], int depth, bool maxPlayer, char turn) {
  printf("grace is dumb");
  int posEval;
  const int n = 8;
  bool valid;
  char oppColour;
  if (turn == 'B') {
    oppColour = 'W';
  } else {
    oppColour = 'B';
  }
  if (depth == 0) {
    posEval = countScore(board);
    return posEval;
  }
  if (maxPlayer) {
    int maxEval = INT_MIN;
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        valid = checkValidMove(board, n, turn, row, col);
        if (valid) {
          char tempBoard[26][26];
          copyBoard(board, tempBoard, n);
          flipPiece(tempBoard, n, turn, row, col);
          posEval = minimax(tempBoard, depth - 1, false, oppColour);
          if (posEval > maxEval) {
            maxEval = posEval;
          }
        }
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        valid = checkValidMove(board, n, turn, row, col);
        if (valid) {
          char tempBoard[26][26];
          copyBoard(board, tempBoard, n);
          flipPiece(tempBoard, n, turn, row, col);
          posEval = minimax(tempBoard, depth - 1, true, oppColour);
          if (posEval < minEval) {
            minEval = posEval;
          }
        }
      }
    }
    return minEval;
  }
}

void copyBoard(char board[][26], char tempBoard[][26], int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      tempBoard[row][col] = board[row][col];
    }
  }
}

int countScore(char tempBoard[][26]) {
  int whitePieces = 0, blackPieces = 0;
  int difference;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (tempBoard[row][col] == 'W') {
        whitePieces++;
      } else if (tempBoard[row][col] == 'B') {
        blackPieces++;
      }
    }
  }
  difference = blackPieces - whitePieces;
  return difference;
}