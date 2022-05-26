/*
 * This is the console program that solve the 9x9 Soduku quiz.
 * It uses the Backtracking algorithm to solve the Sudoku quiz.

 * Written by: Trần Quang Mạnh.
 * 8:46 PM Tuesday, 24-05-2022.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

//User-define data type

/*
 * This data type store the coordinates of a point.
 *
 * Constructor:
 *  Point(const int& xIn = 0, const int& yIn = 0)
 *  Parameter meaning:
 *   xIn: initialization value for x (default value is 0)
 *   yIn: initialization value for y (default value is 0)
 *
 * Variables meaning:
 *  x: The coordinates on horizontal axis.
 *  y: The coordinates on vertical axis.
 */
struct Point {
  Point(const int& xIn = 0, const int& yIn = 0) {
    x = xIn;
    y = yIn;
  }

  int x;
  int y;
};

/*
 * This data type store the point at the top and bottom of a block in the Sudoku quiz.
 *
 * Constructor:
 *  Block(const int& x = 0, const int& y = 0)
 *  Parameter meaning:
 *   x: Coordinate point on the horizontal axis at the top left of the block.
 *   y: Coordinate point on the vertical axis at the top left of the block.
 *
 * Meaning of variables:
 *  top: The point at the top left of the block.
 *  bottom: The point at the bottom right of the block.
 */
struct Block {
  Block(const int& x = 0, const int& y = 0) {
    assert(x <= 9 && y <= 9);
    top = Point(x, y);
    bottom = Point(x + 2, y + 2);
  }

  Point top;
  Point bottom;
};

//All global variables
const int QUIZ_SIZE = 10; //Store the Sudoku quiz size (Index starts at 1).
Point currentCell(1, 1); //Store the position of cell is being solved.
int sudokuTable[QUIZ_SIZE][QUIZ_SIZE]; //Store Sudoku quiz.
Block blockList[QUIZ_SIZE]; //Store the position list of blocks in Sudoku quiz.
int used[QUIZ_SIZE]; //Store the number of times a number occurs (each number can only be used up to nine times).
bool haveAnswer = false; //True if the quiz has answer, else false.
int numAnswer = 0; //Store the number of answers available.
std::string quizFileName = ""; //Store the quiz filename.

//All functions prototypes

//Functions to initialize global variables.
/*
 * This function initializes the coordinate of all blocks (there are 9 blocks) in the Sudoku quiz.
 * No parameters needed.
 * Store values in array blockList.
 * Do not return anything.
 */
void initBlocks();
/*
 * This function initializes the array to count the number of times a number is used.
 * No parameters needed.
 * Store values in array used.
 * Do not return anything.
 */
void initUsed();

//Functions to check if the value is correct in current cell
/*
 * Check whether the value is correct or not in the current row.
 * Parameter: the value needs to check.
 * Return true if value is correct, else return false.
 */
bool checkRow(const int& value);
/*
 * Check whether the value is correct or not in the current collum.
 * Parameter: the value needs to check.
 * Return true if value is correct, else return false.
 */
bool checkCol(const int& value);
/*
 * Check whether the value is correct or not in the current block.
 * Parameter: the value needs to check.
 * Return true if value is correct, else return false.
 */
bool checkBlock(const int& value);
/*
 * Check whether the value is correct or not in row, collum and block.
 * Parameter: the value needs to check.
 * Return true if value is correct, else return false.
 */
bool checkAll(const int& value);

//Function solves Sudoku quiz.
/*
 * This function is used the Backtracking algorithm to solve the Sudoku quiz.
 * If it find the answer for that Sudoku quiz, it will write it to file.
 * Parameter: the cell need to solve.
 * Do not return anything.
 */
void solve(Point cell);

//Functions relative to the file
/*
 * This function is used to read Sudoku quiz in a file *.quiz.
 * Parameter: direction to that file.
 * If can't read file will cancel the program with exit code: -2.
 * Exit code: -1 if the file different from *.quiz.
 * Do not return anything.
 */
void readQuiz(const std::string& dir);
/*
 * This function is used to write the answer for Sudoku quiz in a file *.quiz.
 * Parameter: fileName the name which will be setted for answer file. (default: quizFileName without extension).
 * If can't read file will cancel the program with exit code: -3.
 * Do not return anything.
 */
void writeAnswer(const std::string& fileName = quizFileName);

//Other functions
/*
 * This function is used to find the block in which the current cell is.
 * Parameter: the cell need to find (default value is currentCell).
 * Return the block in which the current cell is.
 */
Block findBlock(const Point& cell = currentCell);
/*
 * This function is used to get the filename from file direction.
 * Parameter:
 *  dir: file direction.
 *  haveExtension: true if you want to get file name and extension, else pass false.
 * Returns filename.
 */
std::string getFileName(const std::string& dir, const bool& haveExtension = true);
/*
 * This function is used to get the file extension from file direction.
 * Parameter:
 *  dir: file direction.
 * Returns file extension.
 */
std::string getFileExtension(const std::string& dir);
/*
 * This function finds and stores the coordinate of the next empty cell that needs to solve.
 * If current cell is not solved, this function will do nothing.
 * No parameters needed.
 * Changes currentCell variable.
 * Returns true if the cell is at the end of the Sudoku table, else returns false.
 * If this function returns false (the cell is at the end of the Sudoku table), it is the
 * answer for that Sudoku quiz.
 */
bool nextEmptyCell();

//Main function
//Program will start here.
int main(int argc, char const *argv[]) {
  quizFileName = "test1213.quiz";

  initBlocks();
  initUsed();
  readQuiz(quizFileName);
  nextEmptyCell();
  solve(currentCell);

  if (!haveAnswer) {
	  std::cout << "Opps!" << "\n\n";
  }
  else {
    std::cout << "\t (!) INFO: Found " << numAnswer << " answer(s)!" << '\n';
  }
  return 0;
}

//All functions define.

void initBlocks() {
  int x = 1, y = 1;

  for (int i = 1; i < QUIZ_SIZE; i++) {
    blockList[i] = Block(x, y);

    if (y < QUIZ_SIZE - 1 - 3) {
      y += 3;
    }
    else {
      x += 3;
      y = 1;
    }
  }
}
void initUsed() {
  for (int i = 1; i < QUIZ_SIZE; i++) {
    used[i] = 0;
  }

  for (int i = 1; i < QUIZ_SIZE; i++) {
    for (int j = 1; j < QUIZ_SIZE; j++) {
      if (sudokuTable[i][j] != 0) {
        used[sudokuTable[i][j]]++;
      }
    }
  }
}

bool checkRow(const int& value) {
  int numAppear[QUIZ_SIZE];
  for (int i = 1; i < QUIZ_SIZE; i++) {
    numAppear[i] = 0;
  }
  numAppear[value] = 1;

  for (int x = 1; x < QUIZ_SIZE; x++) {
    if (sudokuTable[x][currentCell.y] != 0) {
          numAppear[sudokuTable[x][currentCell.y]]++;

          if (numAppear[sudokuTable[x][currentCell.y]] > 1) {
            return false;
          }
    }
  }

  return true;
}
bool checkCol(const int& value) {
  int numAppear[QUIZ_SIZE];
  for (int i = 1; i < QUIZ_SIZE; i++) {
    numAppear[i] = 0;
  }
  numAppear[value] = 1;

  for (int i = 1; i < 11; i++) {
    numAppear[i] = 0;
  }
  numAppear[value] = 1;

  for (int y = 1; y < 11; y++) {
    if (sudokuTable[currentCell.x][y] != 0) {
          numAppear[sudokuTable[currentCell.x][y]]++;

          if (numAppear[sudokuTable[currentCell.x][y]] > 1) {
            return false;
          }
    }
  }

  return true;
}
bool checkBlock(const int& value) {
  int numAppear[QUIZ_SIZE];
  for (int i = 1; i < QUIZ_SIZE; i++) {
    numAppear[i] = 0;
  }
  numAppear[value] = 1;

  Block currentBlock = findBlock(currentCell);

  for (int x = currentBlock.top.x; x <= currentBlock.bottom.x; x++) {
    for (int y = currentBlock.top.y; y <= currentBlock.bottom.y; y++) {
      if (sudokuTable[x][y] != 0) {
            ++numAppear[sudokuTable[x][y]];

            if (numAppear[sudokuTable[x][y]] > 1) {
              return false;
            }
      }
    }
  }

  return true;
}
bool checkAll(const int& value) {
  return (checkBlock(value) && checkRow(value) && checkCol(value));
}

void solve(Point cell) {
  for (int value = 1; value < QUIZ_SIZE; value++) {
    if (used[value] < 9 && checkAll(value)) {
      sudokuTable[cell.x][cell.y] = value;
      ++used[value];

      if (nextEmptyCell()) {
        solve(currentCell);
      }
      else {
        haveAnswer = true;
        numAnswer++;
        writeAnswer();
      }

      currentCell = cell;
      sudokuTable[cell.x][cell.y] = 0;
      --used[value];
    }
  }
}

void readQuiz(const std::string& dir) {
  std::string fileExt = getFileExtension(getFileName(dir));
  if (fileExt == "" || fileExt != "quiz") {
    std::cerr << "\t (X) Error: Invalid file format! Type \"SudokuSolver -h\" for more infomation.\n\n";
    exit(-1);
  }

  std::ifstream quiz(dir.c_str(), std::ios::in);
  if (quiz.is_open()) {
    for (int i = 1; i < QUIZ_SIZE; i++) {
      for (int j = 1; j < QUIZ_SIZE; j++) {
        quiz >> sudokuTable[i][j];
      }
    }
    quiz.close();
  }
  else {
    std::cerr << "\t (X) Error: Unable to open Sudoku quiz file! Please try again.\n\n";
    exit(-2);
  }
}
void writeAnswer(const std::string& fileName) {
  const std::string ext = ".solved";
  const std::string file = getFileName(fileName, false) + "_" + std::to_string(numAnswer) + ext;

  std::ofstream ans(file, std::ios::out);
  if (ans.is_open()) {
    for (int x = 1; x < QUIZ_SIZE; x++) {
      for (int y = 1; y < QUIZ_SIZE; y++) {
        ans << sudokuTable[x][y] << " ";
      }
      ans << "\n";
    }
    ans.close();
  }
  else {
    std::cerr << "\t (X) Error: Can't write answer file!" << "\n\n";
    exit(-3);
  }
}

Block findBlock(const Point& cell) {
  for (int i = 1; i < 11; i++) {
    if (blockList[i].top.x <= cell.x && cell.x <= blockList[i].bottom.x) {
      if (blockList[i].top.y <= cell.y && cell.y <= blockList[i].bottom.y) {
        return blockList[i];
      }
    }
  }

  return Block();
}
std::string getFileName(const std::string& dir, const bool& haveExtension) {
  int pos = dir.find_last_of("/\\");
  std::string result = "";

  if (haveExtension) {
    result = dir.substr(pos + 1);
  }
  else {
    int pos = dir.find_last_of(".");
    result = dir.substr(0, pos);
  }

  return result;
}
std::string getFileExtension(const std::string& dir) {
  int pos = dir.find_last_of(".");
  return dir.substr(pos + 1);
}
bool nextEmptyCell() {
  if (sudokuTable[currentCell.x][currentCell.y] == 0) {
    return true;
  }

  if (currentCell.x == 9 && currentCell.y == 9) {
    return false;
  }

  while (currentCell.x < QUIZ_SIZE) {
    while (currentCell.y < QUIZ_SIZE - 1) {
      currentCell.y++;

      if (sudokuTable[currentCell.x][currentCell.y] == 0) {
        return true;
      }
    }
    currentCell.x++;
    currentCell.y = 0;
  }

  return false;
}
