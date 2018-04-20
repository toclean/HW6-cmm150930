/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include "cdk.h"
#include <string>
#include <sstream>
#include <stdint.h>
#include <fstream>

#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 3
#define BOX_WIDTH 17
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

class BinaryFileHeader
{
	public:
		uint32_t magicNumber;
		uint32_t versionNumber;
		uint64_t numRecords;
};

const int maxRecordStringLength = 25;

class BinaryFileRecord
{
	public:
		uint8_t strLength;
		char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  const char 		*rowTitles[MATRIX_HEIGHT+1] = {"0", "a", "b", "c"};
  const char 		*columnTitles[MATRIX_WIDTH+1] = {"0", "a", "b", "c", "d", "e"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT,
			  MATRIX_NAME_STRING, (char **) columnTitles, (char **) rowTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix == NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   *
   * setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
   */

	BinaryFileHeader fileHeader;
	ifstream infile ("cs3377.bin", ios::binary | ios::in);
	infile.read((char*)&fileHeader, sizeof(fileHeader));

	stringstream ss;
	ios_base::fmtflags oldFlags = ss.flags();
	ss << "Magic: " << "0x" << uppercase << hex << fileHeader.magicNumber;
	string mn = ss.str();
	ss.str("");
	ss.clear();
	ss.flags(oldFlags);
	ss << "Version: " << fileHeader.versionNumber;
	string vn = ss.str();
	ss.str("");
	ss.clear();
	ss << "NumRecords: " << fileHeader.numRecords;
	string nr = ss.str();
	ss.str("");
	ss.clear();

	setCDKMatrixCell(myMatrix, 1, 1, mn.c_str());
	setCDKMatrixCell(myMatrix, 1, 2, vn.c_str());
	setCDKMatrixCell(myMatrix, 1, 3, nr.c_str());
	drawCDKMatrix(myMatrix, true);    /* required  */

	BinaryFileRecord fileRecord;

	int i = 2;

	while (infile.read((char*)&fileRecord, sizeof(fileRecord)))
	{
		ss << "strlen: " << (int)fileRecord.strLength;
		string sl = ss.str();
		ss.str("");
		ss.clear();
		ss.flags(oldFlags);
		ss << fileRecord.stringBuffer;
		string sb = ss.str();
		ss.str("");
		ss.clear();

		cout << i;

		setCDKMatrixCell(myMatrix, i, 1, sl.c_str());
		setCDKMatrixCell(myMatrix, i, 2, sb.c_str());
		i++;
	}

	drawCDKMatrix(myMatrix, true);

  /* so we can see results */
  sleep (3);


  // Cleanup screen
  endCDK();
}
