/*
 * Connor Moore
 * cmm150930@utdallas.edu
 * CS 3377.002
 * Adapted code provided by Dr. Perkins
 */

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
#include <iomanip>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 17
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

// Class for storing file header
class BinaryFileHeader
{
	public:
		uint32_t magicNumber;
		uint32_t versionNumber;
		uint64_t numRecords;
};

const int maxRecordStringLength = 25;

// Class for storing file record
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

  const char 		*columnTitles[] = {"0", "a", "b", "c"};
  const char 		*rowTitles[] = {"0", "a", "b", "c", "d", "e"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED};

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
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

	// Checking if the matrix was created
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


	// Creates a new fileHeader and opens the file for binary reading
	BinaryFileHeader fileHeader;
	ifstream infile ("cs3377.bin", ios::binary | ios::in);
	infile.read((char*)&fileHeader, sizeof(fileHeader));

	// Chunk of code that adds the contents from the header to variables
	stringstream ss;
	ios_base::fmtflags oldFlags = ss.flags();
	ss << "Magic: " << "0x" << uppercase << hex << setw(8) << fileHeader.magicNumber;
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

	// Adds information from the header to the matrix
	setCDKMatrixCell(myMatrix, 1, 1, mn.c_str());
	setCDKMatrixCell(myMatrix, 1, 2, vn.c_str());
	setCDKMatrixCell(myMatrix, 1, 3, nr.c_str());
	drawCDKMatrix(myMatrix, true);    /* required  */

	BinaryFileRecord fileRecord;

	int i = 2;
	int j = 0;

	// Reads the file records and adds their contents to the matrix
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

		if (j < 5){
		    setCDKMatrixCell(myMatrix, i, 1, sl.c_str());
		    setCDKMatrixCell(myMatrix, i, 2, sb.c_str());
		}
		j++;
		i++;
	}

	// Draw the matrix
	drawCDKMatrix(myMatrix, true);

  /* so we can see results */
  unsigned char x;
	cin >> x;

  // Cleanup screen
  endCDK();

	// Close the file
	infile.close();
}
