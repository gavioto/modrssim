// FILE: CSVReader.cpp
//
// Mod_RSSim (c) Embedded Intelligence Ltd. 1993,2009
// AUTHOR: Conrad Braam.  http://www.plcsimulator.org
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as
//    published by the Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/* class CCSVTextImporter
 *         the class that imports a 17-column CSV file
 * [internal]
 * class CCSVTextLine implementation
 *       CCSVTextFile implementation
 *       CCSVLineArray implementation
 *
 *
 *****************************************************************************/

#include "stdafx.h"
#include "resource.h"


//IMPLEMENT_DYNAMIC( CCSVTextLine, CString)

//////////////////////////////////////////////////////////////////////////////
// class CCSVTextLine
// implementation

// this class is an extension of CString, all it does is contruct from 
// a standard string. Then it allows you to interrogate the string which
// is seperated CSV style.
// --------------------------- CCSVTextLine::CCSVTextLine ------------------
CCSVTextImporter::CCSVTextLine::CCSVTextLine(LPCSTR string) : CString (string)
{
   m_init = FALSE;
   Parse();
}

CCSVTextImporter::CCSVTextLine::CCSVTextLine(CCSVTextLine& other)
{
   this->m_init = other.m_init;
   this->m_instrumentNum = other.m_instrumentNum;
   memcpy(this->m_values, other.m_values, sizeof(other.m_values));
}


// ----------------------------------- CCSVTextLine -------------------------
CCSVTextImporter::CCSVTextLine::CCSVTextLine() : CString ()
{
   m_init = FALSE;
} // CCSVTextLine

// ----------------------------------- GetElement ----------------------------
// return values in a row
double CCSVTextImporter::CCSVTextLine::GetElement(LONG index)
{
   ASSERT((index >=0) && (index <= MAX_CSVFILE_COLUMNS));
   return(m_values[index]);
}


// ----------------------------------- operator= -----------------------------
// compare operator for a other line
CCSVTextImporter::CCSVTextLine& CCSVTextImporter::CCSVTextLine::operator=(CCSVTextLine &otherLine)
{
   *((CString*)(this)) = *(CString*)&otherLine;
   return(*this);
} // operator=

// ----------------------------------- operator= -----------------------------
// compare operator for a CString
CCSVTextImporter::CCSVTextLine& CCSVTextImporter::CCSVTextLine::operator=(CString &otherString)
{
   *((CString*)(this)) = otherString; 
   return(*this);
} // operator=


// ----------------------------------- Parse ---------------------------------
// Fetch the variables out of a line of text
//
// This function uses a technique I call indirect addressing.
// it sets up a pointer to one variable, then increments it to point to the next.
// The trick is that the varialbes are not in an array, but just coded 
// consecutively so that the function does not have to take ages un-packing, since 
// this routine would get called many times.
//
void CCSVTextImporter::CCSVTextLine::Parse()
{
LONG  pos, last, next, fields=0;
CString temp;
double * varPtr = &m_values[0];
   
   m_init = TRUE;
   pos = Find(',');
   if (-1 == pos)
      return;
   last = pos+1;
   temp = Mid(0, pos);  //instrument#
   sscanf(temp, "%d", &m_instrumentNum);
   fields++;
   *varPtr++ = m_instrumentNum;

   next = pos;
   // get the other fields in a loop
   while ((-1 != last)&&(-1 != next))
   {
      temp = Mid(last, GetLength()-last);
      next = temp.Find(',');
      if (-1!=next)
         temp = Mid(last, next);

      sscanf(temp, "%lg", varPtr);
      OutputDebugString(temp);
      OutputDebugString(" ");
      last = last+next+1;
      fields++;
      varPtr++;
      if (fields > MAX_CSVFILE_COLUMNS)
      {
         ASSERT(0);
         break;  // forced break
      }
   }
   ASSERT(fields == MAX_CSVFILE_COLUMNS);
   OutputDebugString("\n");
} // Parse



//////////////////////////////////////////////////////////////////////////////
// class CCSVTextFile
// implementation

// PURPOSE: read the CSV text file in in one go.
// this class gets deleted after it has been used, in order to conserve RAM
// the contents get copied into an array.
CCSVTextImporter::CCSVTextFile::CCSVTextFile(LPCTSTR fileName, UINT flags) : CFile(fileName, flags)
{
   m_length = (DWORD)GetLength();
   m_data = (BYTE*)malloc(m_length + 1);

   Read((void*)m_data, m_length);
   m_data[m_length] = '\0';
}

// ----------------------------------- ~CCSVTextFile ------------------------------
CCSVTextImporter::CCSVTextFile::~CCSVTextFile()
{
   if (NULL != m_data)
      free(m_data);
} // ~CCSVTextFile

// ----------------------------------- Data ----------------------------------
// return a pointer to the data, this pointer becomes invalid once this 
// object is destroyed.
LPCTSTR CCSVTextImporter::CCSVTextFile::Data()
{
   return((LPCTSTR)m_data);
} // Data

//////////////////////////////////////////////////////////////////////////////
// class CCSVLineArray
// implementation

// ----------------------------------- ~CCSVLineArray --------------------------
CCSVTextImporter::CCSVLineArray::~CCSVLineArray()
{
CCSVTextLine * pLine;
LONG index, size = GetSize();

   if (size)
   {
      for (index = 0; index < size; index++)
      {
         pLine = GetAt(index);
         delete pLine;
      }
   }
   RemoveAll();
} // ~CCSVLineArray

// ----------------------------------- GetAt ---------------------------------
CCSVTextImporter::CCSVTextLine* CCSVTextImporter::CCSVLineArray::GetAt(LONG index)
{
   return((CCSVTextImporter::CCSVTextLine*)CObArray::GetAt(index));
} // GetAt

// ----------------------------------- Add -----------------------------------
LONG CCSVTextImporter::CCSVLineArray::Add(CCSVTextImporter::CCSVTextLine *pLine)
{
   CCSVTextImporter::CCSVTextLine lineA(*pLine);
   //lineA
   return(CObArray::Add((CObject*)pLine));
} // // ----------------------------------- Add


// ----------------------------------- operator [] ---------------------------
CCSVTextImporter::CCSVTextLine *CCSVTextImporter::CCSVLineArray::operator [](LONG index)
{
   return(GetAt(index));
} // operator []

//////////////////////////////////////////////////////////////////////////////
// class CCSVTextImporter
// implementation

// Wrapper class which uses all of the above classes to load the CSV file
// in CSV format. The constructor automaticaly loads the file and puts it 
// into an array.
CCSVTextImporter::CCSVTextImporter()
 : CObject()
{
   myArray = NULL;
   memset(&m_lastInterval, 0, sizeof(m_lastInterval));
}

CCSVTextImporter::~CCSVTextImporter()
{
   if (myArray)
      delete myArray;
}


void CCSVTextImporter::Open(LPCTSTR fileName)
{
CCSVTextFile * file;
CString data;
LONG pos,curPos;

   if (myArray)
      delete myArray;
   myArray = new CCSVLineArray;
   TRY
   {
      file = new CCSVTextFile(fileName, CFile::modeRead| CFile::shareDenyNone);
      data = file->Data();
      pos = data.Find('\n');
      if (-1 != pos)
      {
         // strip off the column headings
         data = data.Mid(pos+1, data.GetLength()-pos-1);

         while (-1 != pos)
         {
         CString curLine;
            pos = data.Find('\n');
            if (-1 != pos)
            {
               curLine = data.Mid(0, pos-1);
               data = data.Mid(pos+1, data.GetLength()-pos-1);
            }
            else
               curLine = data;
            // make double sure this is just one line
            curPos = curLine.Find('\n');
            if (-1 != curPos)
            {
               curLine = curLine.Mid(0, curPos-1);
            }
            if (curLine.GetLength())
            {
               // put the line into the array
               CCSVTextLine *pString;
                  pString = new CCSVTextLine(curLine);
                  //*pString = curLine;
               myArray->Add(pString);
            }
         }
      }
      //printf(data);
      delete file;
   }
   CATCH (CFileException, e)
   {
   CHAR msg[MAX_DEBUG_STR_LEN];   

      sprintf(msg, "Error %d opening CSV file", e->m_cause);
      OutputDebugString(msg);
   }
   END_CATCH

   // initialisation completed 
} // CCSVTextImporter

// ----------------------------------- LoadedOK ------------------------------
BOOL CCSVTextImporter::LoadedOK()
{
   if (myArray->GetSize())
      return(TRUE);
   return(FALSE);
} // LoadedOK

// ----------------------------------- HandleTimer ----------------------------
// return TRUE if we have data to dump into registers
//
BOOL CCSVTextImporter::HandleTimer(LPCTSTR importFolder, CRegisterUpdaterIF *pParentInterface)
{

   m_parentInterface = pParentInterface;
   ASSERT(m_parentInterface );
   
   // Verify that my invented data algorithm is same as Vinay's approach.
   {
	//COleDateTime dtCurrent	      =	COleDateTime::GetCurrentTime();
	//CString CurrentDate		=	dtCurrent.Format("%Y%m%d");
	//CString CurrentTime		=	dtCurrent.Format("%H%M");

	//CString strTime  = CurrentTime.Mid(2,2);
	//CString strFile;
   }

   //Test for new CSV file
   CString fileName;
   SYSTEMTIME  sysTimeExpect, currentTime;
   CString fullFileName;

      GetLocalTime(&currentTime);
      sysTimeExpect = currentTime;

      sysTimeExpect.wMinute = (sysTimeExpect.wMinute/15)*15;
      fileName.Format("%04d%02d%02d\\%02d%02d.csv", sysTimeExpect.wYear, 
                                                    sysTimeExpect.wMonth, 
                                                    sysTimeExpect.wDay, 
                                                    sysTimeExpect.wHour, 
                                                    sysTimeExpect.wMinute);
      fullFileName = importFolder;
      fullFileName+= '\\';
      fullFileName+= fileName;

      // prevent re-processing the same file twice
      // todo: this may need to be cleverer, although it is not a real problem on start-up either.
      if ((fileName != m_lastProcessed) &&(ExistFile(fullFileName)))
      {
      CWaitCursor wait;    // put up a wait cursor

         if (ImportFile(fullFileName))
         {
            m_lastProcessed = fileName;
            return(true);
         }
      }
   if (fileName != m_lastProcessed)
   {
      // check for no CSV found/skipped cases
      CTime    localTime(currentTime);
      CTime    lastTime(m_lastInterval);
      CTimeSpan elapsedTime = localTime - lastTime;
      LONGLONG stale = elapsedTime.GetTotalMinutes();
   
      if (stale > 15)
      {
         m_lastInterval = sysTimeExpect;
         CString msg;
         msg.Format("The expected CSV import file '%s' was not found!", fullFileName);
         m_parentInterface->DebugMessage(msg);
      }
   }
   return(false);
} // HandleTimer


// --------------------------- ImportFile -----------------------------
// returns: TRUE if the file was read without error, else FALSE
//
LONG CCSVTextImporter::ImportFile(LPCTSTR csvName, BOOL manual, CRegisterUpdaterIF *pParentInterface/* =0*/)
{
CString msg, msgFormat;

   if (pParentInterface)
      m_parentInterface = pParentInterface;
   ASSERT(m_parentInterface);
   if (!manual)
      msgFormat.LoadString(IDS_MSGCSVSTARTPROCESSING);
   else
      msgFormat.LoadString(IDS_MSGCSVSTARTPROCESSING_MAN);

   msg.Format(msgFormat, csvName);
   m_parentInterface->DebugMessage(msg);
   {
      // Process CSV
      // open file and load all in here
      Open(csvName);
      
      // return if error occurs
      if (!LoadedOK())
      {
         msg.Format("Processing CSV failed.");
         m_parentInterface->DebugMessage(msg);
         return(0);
      }
      // Viola, job done
   }

   msg.Format("End processing OK");
   m_parentInterface->DebugMessage(msg);
   return(1);
}

#define CSV_REGISTERSIZE   4
#define CSV_CLONEMODBUS    (true)

// ------------------------------ UpdateRegisters -----------------------------------
// allows us to call this function to update data a while after reading the file
// so that we can slice up the time used if needed
LONG CCSVTextImporter::UpdateRegisters()
{
LONG count=0;

   ASSERT(m_parentInterface );
   
   // TODO: copy values into modbus registers now. We do this by column, not by row
   // 40000-40068 addresses reserved for the first row


   int col, row, registerNum;
   for (row=0; row < this->LineCount(); row++)
      for (col = 0; col < MAX_CSVFILE_COLUMNS;col++)
      {
         registerNum = (row* CSV_REGISTERSIZE* MAX_CSVFILE_COLUMNS)+ (CSV_REGISTERSIZE*col);
         float value = (float)myArray->GetAt(row)->GetElement(col);
         DWORD lowhigh;
         float*pFloatValue;
         pFloatValue= (float *)&lowhigh;
         *pFloatValue = value;

         if (CSV_CLONEMODBUS)
            SwopWords(&lowhigh);

         m_parentInterface->SetRegister(registerNum, LOWORD(lowhigh));
         m_parentInterface->SetRegister(registerNum+1, HIWORD(lowhigh));
         count++;
      }
   return(count);
}