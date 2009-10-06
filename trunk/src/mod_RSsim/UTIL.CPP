/////////////////////////////////////////////////////////////////////////////
//
// FILE: util.cpp
//
// See _README.CPP
//
/////////////////////////////////////////////////////////////////////////////
//
// Implementation of Utility functions
//     The functions and classes below are defined in "util.h"
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


// ----------------------------- PackField ----------------------------
// PURPOSE: packs the value into ASCII-HEX into the telegram
// NOTE:
//    Unused
void PackField(CHAR **pBuffer, WORD value, WORD length)
{
CHAR tempStr[80];
CHAR formatStr[80];

   // build a format string
   sprintf(formatStr, "%%0%dX", length);
   // plonk it in
   sprintf(tempStr, formatStr, value);
   strncpy(*pBuffer, tempStr, length);
   // increment pointer
   *pBuffer+=length;
   **pBuffer = '\0'; // add a null
} // PackField


// ------------------------------ SwapBytes ----------------------------
// PURPOSE: Swap the HI and LO bytes of a WORD
// NOTES: This could have been written as a macro, but gets messy when using 
//        pointer de-refferencing and type casting.
WORD SwapBytes(WORD wrd)
{
    WORD result;

    result = ( (LOBYTE(wrd) << 8) + HIBYTE(wrd) ); 
    return(result);
} // SwapBytes


// -------------------------------- UnPackField --------------------------
// return a WORD or BYTE (depends on length) at a pointer
WORD UnPackField(BYTE **pBuffer, WORD length)
{
BYTE *pField = (BYTE*)(*pBuffer);
WORD value=0;
int i;
      
   for (i=0;i<length;i++)
   {
      //**** 20021101 mod by HF
	   //value = value * 16;
	   value *= 256;
	  //**** 20021101 end
      value += (BYTE)*pField;
      pField++;
   }
   // increment pointer
   *pBuffer +=length;
   return (value);
} // UnPackField

// ------------------------- UnPackASCIIField -------------------------------
WORD UnPackASCIIField(BYTE **pBuffer, WORD length, BOOL & error)
{
CHAR *pField = (CHAR*)(*pBuffer);
WORD value=0;
int i;

   error = FALSE;      
   for (i=0;i<length;i++)
   {
      value = value * 16;
      //value += ( (*pField >= 'A') ? 
      //            (*pField - ('A' - 10) ) : (*pField - '0') );
      if ((*pField >= 'A')&&(*pField <= 'F'))
         value += *pField - ('A' - 10);
      else
         if ((*pField >= 'a')&&(*pField <= 'f'))
            value += *pField - ('a' - 10);
         else
         {
            if ((*pField >= '0')&&(*pField <= '9'))
               value += *pField - '0';
            else
               error = TRUE;
         }
      pField++;

   }
   // increment pointer
   *pBuffer+=length;
   return (value);
}

/*
// ------------------------------ FillCharCBox ----------------------
// fill a combo-box control from a string array. the user-Data in the 
// combo is filled from an array of DWORDs.
void FillCharCBox(CComboBox * cBox, DWORD * table, char ** strTable,
                  WORD tableLen, DWORD currentsetting)
{
DWORD count;
CHAR ** strTablePtr = strTable;

   cBox->ResetContent();
   for (count = 0; count < tableLen; count++)
   {
      cBox->AddString(strTablePtr[count]);
      cBox->SetItemData(count,  *(table + count));
      if (*(table + count) == currentsetting)
         cBox->SetCurSel(count);
   }
} // FillCharCBox

// --------------------------- FillDWordCBox --------------------------
// fill a combo-box with numbers from a table.
void FillDWordCBox(CComboBox * cBox, DWORD * table, WORD tableLen,
                   DWORD currentsetting)
{
DWORD count;
CHAR temp[256];

   cBox->ResetContent();
   for (count = 0; count < tableLen; count++)
   {
      sprintf(temp,"%ld",(DWORD *)table[count]);
      cBox->AddString(temp);  //strTablePtr[count]);
      cBox->SetItemData(count,  *(table + count));
      if (*(table + count) == currentsetting)
         cBox->SetCurSel(count);
   }
} // FillDWordCBox


///////////////////////////////////////////////////////////////////////
// Local constants and function defs
#define PORT_INUSESTR      " *"     // appended to LB for used comm ports

extern BOOL PortInUse(LPCTSTR portName);
extern void FillSerialCBoxUsedResources(CComboBox * cBox, LPCTSTR currentselection);
extern void ClearPortUseMark(LPSTR name);

// --------------------------- EnumerateSerialPorts -----------------------------
// PURPOSE: Retrieve hardware configuration from registry instead of letting
//          the user guess what ports he has available.
//
LONG EnumerateSerialPorts (char *deviceName, 
                           DWORD maxLen, 
                           DWORD index)
{
CHAR     RegPath[MAX_PATH]  = "HARDWARE\\DEVICEMAP\\SERIALCOMM";
HKEY     hKey;
HKEY     hKeyRoot = HKEY_LOCAL_MACHINE;
DWORD    retCode;
CHAR     ClassName[MAX_PATH] = ""; // Buffer for class name.
DWORD    dwcClassLen = MAX_PATH;   // Length of class string.
DWORD    dwcSubKeys;               // Number of sub keys.
DWORD    dwcMaxSubKey;             // Longest sub key size.
DWORD    dwcMaxClass;              // Longest class string.
DWORD    dwcValues;                // Number of values for this key.
CHAR     valueName[MAX_VALUE_NAME] ;
DWORD    dwcValueName = MAX_VALUE_NAME;
DWORD    dwcMaxValueName;          // Longest Value name.
DWORD    dwcMaxValueData;          // Longest Value data.
DWORD    dwcSecDesc;               // Security descriptor.
FILETIME ftLastWriteTime;          // Last write time.
DWORD    dwType;
DWORD    retValue;
DWORD    cbData;


   // Use RegOpenKeyEx() with the new Registry path to get an open handle
   // to the child key you want to enumerate.
   retCode = RegOpenKeyEx (hKeyRoot,
                           RegPath,
                           0,
                           KEY_ENUMERATE_SUB_KEYS |
                           KEY_EXECUTE |
                           KEY_QUERY_VALUE,
                           &hKey);

   if (retCode != ERROR_SUCCESS)
      return(aFAILED);


   // Get Class name, Value count.
   RegQueryInfoKey ( hKey,              // Key handle.
                     ClassName,         // Buffer for class name.
                     &dwcClassLen,      // Length of class string.
                     NULL,              // Reserved.
                     &dwcSubKeys,       // Number of sub keys.
                     &dwcMaxSubKey,     // Longest sub key size.
                     &dwcMaxClass,      // Longest class string.
                     &dwcValues,        // Number of values for this key.
                     &dwcMaxValueName,  // Longest Value name.
                     &dwcMaxValueData,  // Longest Value data.
                     &dwcSecDesc,       // Security descriptor.
                     &ftLastWriteTime); // Last write time.

   // Enumerate the Key Values
   cbData = maxLen ;
   dwcValueName = MAX_VALUE_NAME;
   valueName[0] = '\0';

   retValue = RegEnumValue (hKey, index, valueName,
                            &dwcValueName,
                            NULL,
                            &dwType,
                            (BYTE *)&deviceName[0],
                            &cbData);

   RegCloseKey (hKey);   // Close the key handle.
   if(dwType == REG_SZ && retValue == (DWORD)ERROR_SUCCESS)
      return(SUCCESS);
   else
      return(aFAILED);
} // EnumerateSerialports


// ---------------------------- PortInUse ------------------------------
// Returns whether the RS-232 port can currently be opened.
// It tests this by opening and closing the port.
BOOL PortInUse(LPCTSTR portName)
{
CHAR port[MAX_PORT_NAME];
HANDLE   hPort;

   GetLongComPortName(portName, port);
   hPort =  CreateFile( port,                    // PIP 1823-400-1997
                        GENERIC_READ | GENERIC_WRITE,
                        (DWORD)NULL,   // exclusive access
                        NULL,          // no security
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL           // hTemplate
                      );
   if (INVALID_HANDLE_VALUE != hPort)
   {
      CloseHandle(hPort);
      return (FALSE);
   }
   return (TRUE);
} // PortInUse
*/

// -------------------------------- PortAvailable ---------------------------
// This func simply calls the other func
BOOL PortAvailable(LPCTSTR portName)
{
   return (!PortInUse(portName));
} // PortAvailable

/*
// --------------------------- FillSerialCBoxUsedResources ------------------
// Calls the function FillSerialCBox(), and then puts a '*' next to all used
// ports.
void FillSerialCBoxUsedResources(CComboBox * cBox, LPCTSTR currentselection)
{
DWORD index, count;
LONG selection;
CString selectionText;
   FillSerialCBox(cBox, currentselection);
   count = cBox->GetCount();
   selection = cBox->GetCurSel();
   index = 0;
   while (index < count)
   {
      cBox->GetLBText(index, selectionText);
      if (PortInUse(selectionText))
      {
         cBox->DeleteString(index);
         selectionText = selectionText + PORT_INUSESTR;  // " *"
         cBox->InsertString(index, selectionText);
         cBox->SetItemData(index, index);
      }
      index++;
   }
   cBox->SetCurSel(selection);
} // FillSerialCBoxUsedResources


// ---------------------------- ClearPortUseMark -------------------------
// Removes the '*' mark from a port name if present.
// Use this function to read the LB contents if U used FillSerialCBoxUsedResources()
void ClearPortUseMark(LPSTR name)
{
   strtok(name, PORT_INUSESTR);
} // ClearPortUseMark


// ----------------------------- FillSerialCBox -----------------------------
// the list box contains the strings that describe each serial comm port available
// the port names themselves are retrieved when destroying the combo box
void FillSerialCBox(CComboBox * cBox, LPCTSTR currentselection)
{
DWORD count;
LONG retCode;
CHAR portname[MAX_PORT_NAME];

   cBox->ResetContent();
   count = 0;
   while (TRUE)
   {
      retCode = EnumerateSerialPorts(portname, sizeof(portname), count);
      if( retCode != SUCCESS)
         break;
      cBox->AddString(portname);
      cBox->SetItemData(count, count);
      if (strcmp(portname,currentselection)==0)
         cBox->SetCurSel(count);
      count++;
   }
} // FillSerialCBox
*/

// ----------------------------- GetFirstFreePort -----------------------------
// Go thru all ports, and find the 1st unused one
void GetFirstFreePort(LPSTR freePortName)
{
DWORD count;
LONG retCode;
CHAR portname[MAX_PORT_NAME];

   freePortName[0] = '\0';
   count = 0;
   while (TRUE)
   {
      retCode = EnumerateSerialPorts(portname, sizeof(portname), count);
      if( retCode != SUCCESS)
         break;
      if (PortAvailable(portname))
      {
         strcpy(freePortName, portname);
         return;
      }
      // try next port
      count++;
   }
} // GetFirstFreePort



// ----------------------------- LoadDATAResource -------------------------------
// load a TEXT data resource into a CString
BOOL LoadDATAResource(int resourceID, char **pBuffer, DWORD* resSize)
{
HRSRC hFound;
HGLOBAL   hRes;
BOOL bResult;	
DWORD size;
HMODULE hModule = AfxGetResourceHandle();
   
   hFound = FindResource(hModule, MAKEINTRESOURCE(resourceID), "HELPFILES");
   hRes = LoadResource(hModule, hFound);
   size = SizeofResource(hModule, hFound);
   if (size)
   {
      *pBuffer = new char[size+1];
      if (NULL !=*pBuffer)
      {
         memcpy(*pBuffer, LockResource(hRes), size);
         (*pBuffer)[size] = '\0';   // terminate the string
      }
      bResult = UnlockResource(hRes);
   }
   else 
      return (FALSE);
   bResult = FreeResource(hRes); 
   *resSize = size;
   return (TRUE);
} // LoadDATAResource


BOOL FindFragment(const BYTE * buffer, DWORD length, 
                  const BYTE *pSearchBuff, DWORD mSearchLength, 
                  DWORD *pos)
{
BOOL found = FALSE;
DWORD idx=0;

   while (idx + mSearchLength <= length)
   {
      if (0 ==memcmp(&buffer[idx], pSearchBuff, mSearchLength))
      {
         *pos = idx;
         found = TRUE;
         break;
      }
      idx++;
   }
   return(found);
}