/////////////////////////////////////////////////////////////////////////////
//
// FILE: MODCommsProcessor.cpp : implementation file
//
// See "_README.CPP"
//                    
// implementation of the CMODCommsProcessor class.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mod_rssimDlg.h"
#include "message.h"
#include "abCommsProcessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC( CCommsProcessor, SimulationSerialPort);


CCommsProcessor::CCommsProcessor(LPCTSTR    portNameShort, 
                     DWORD    baud, 
                     DWORD    byteSize, 
                     DWORD    parity, 
                     DWORD    stopBits,
                     DWORD    rts,
                     int      responseDelay)
{
   InitializeCriticalSection(&stateCS);
   
}

CCommsProcessor::~CCommsProcessor()
{

}





// ------------------------------- ActivateStationLED ---------------------------
void CCommsProcessor::ActivateStationLED(LONG stationID)
{
   if (stationID>0 && stationID<STATIONTICKBOXESMAX)
   {
      //start the counter for this station at the beginning
      pGlobalDialog->m_microTicksCountDown[stationID] = pGlobalDialog->GetAnimationOnPeriod();
      // it will count down untill it extinguishes
   }
} // ActivateStation


// ------------------------------ RSDataDebugger ------------------------------
void CCommsProcessor::RSDataDebugger(const BYTE * buffer, LONG length, int transmit)
{
CString debuggerString;
BYTE *data;
byte hiNib,loNib;

   //convert BIN-ary to ASCII for display
   data = new BYTE[(length*2)+1];
   for (int i = 0; i < length; i++)
   {
       hiNib = ( *(buffer+i) >>4) & 0x0f;
       loNib = ( *(buffer+i)    ) & 0x0f;
       data[(i*2)]   = ( (hiNib < 0x0A) ? ('0' + hiNib) : ('A' + hiNib-10) );
       data[(i*2)+1] = ( (loNib < 0x0A) ? ('0' + loNib) : ('A' + loNib-10) );
   }

   data[(length*2)] = '\0';

   if (transmit)
   {
      //if (length)
      //   pGlobalDialog->CharactersSent();
      debuggerString.Format("TX:%s\n", data);
   }
   else
   {
      //if (length)
      //   pGlobalDialog->CharactersReceived();
         debuggerString.Format("RX:%s\n", data);
   }
   // Send to debugger list-box
   if (length)
      pGlobalDialog->AddCommsDebugString(debuggerString);

#ifdef _COMMS_DEBUGGING
   if (length)
      OutputDebugString(debuggerString);
#endif

   delete (data);
} // RSDataDebugger

// ------------------------------- RSStateChanged -----------------------
void CCommsProcessor::RSStateChanged(DWORD state)
{
   EnterCriticalSection(&stateCS);
   pGlobalDialog->m_ServerRSState = state;
   LeaveCriticalSection(&stateCS);
} // RSStateChanged


/////////////////////////////////////////////////////////////////////
//
// class CAB232CommsProcessor
//
IMPLEMENT_DYNAMIC( CAB232CommsProcessor, CPLCApplication);

CAB232CommsProcessor::CAB232CommsProcessor(LPCTSTR portNameShort, 
                                     DWORD  baud, 
                                     DWORD byteSize, 
                                     DWORD parity, 
                                     DWORD stopBits,
                                     DWORD rts,
                                     int   responseDelay,
                                     BOOL  MOSCADchecks,
                                     BOOL modifyThenRespond,
                                     BOOL disableWrites,
									          BOOL bcc,
                                     DWORD masterIdleTime,
                                     DWORD timeoutValue) : CPLCApplication( portNameShort, 
                                                                            baud, 
                                                                            byteSize, 
                                                                            parity, 
                                                                            stopBits,
                                                                            rts,
                                                                            responseDelay,
                                                                            MOSCADchecks,
                                                                            modifyThenRespond,
                                                                            disableWrites,
														                    					bcc)
{
   InitializeCriticalSection(&stateCS);

   m_responseDelay = responseDelay;
   m_ABmasterIDLETime = masterIdleTime;
   m_ABtimeoutValue = timeoutValue;
   SetEmulationParameters(MOSCADchecks, modifyThenRespond, disableWrites);
   m_pWorkerThread->ResumeThread(); //start thread off here

}


// ---------------------------------- Send --------------------------------
// PURPOSE: This one should be clear
//    Also does DLE expansion.
LONG CABCommsProcessor::Send(int numberOfBytestoWrite, 
                            const BYTE* bufferPtr,
                            BOOL insertDLEs, 
                            CHAR* debugStrPtr)
{
DWORD numberOfBytesWritten=0;
      // pointer to buffer to write out on the wire
BYTE* pBuffer = (BYTE*)bufferPtr;
int dleCount=0;
BOOL retValue;

   ASSERT (numberOfBytestoWrite>0);
   if (!keepPolling)
      return FALSE;
   

   // insert DLE's as needed
   if (insertDLEs)
   {
   BYTE *pSource = (BYTE*)bufferPtr, *pDest;
   int count = numberOfBytestoWrite;

      // allocate a new buffer instead
      pBuffer = new byte[numberOfBytestoWrite*2];
      //
      pDest = pBuffer;
      //
      while (count>0)
      {
         if (*pSource == 0x10)
         {
            *pDest++=0x10;       // insert DLE
            dleCount++;

         }
         *pDest++ = *pSource++;
         count--;
      }
      // # of bytes on the wire will increase accordingly
      numberOfBytestoWrite += dleCount;
   }
   retValue = CCommsProcessor::Send(numberOfBytestoWrite, pBuffer, debugStrPtr);
   
   // free up the DLE expanded buffer if one was allocated
   if (insertDLEs)
      delete pBuffer;

   return(retValue);
} // Send

