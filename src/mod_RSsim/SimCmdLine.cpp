// SimCmdLine.cpp: implementation of the CSimCmdLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mod_rssim.h"
#include "SimCmdLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimCmdLine::CSimCmdLine()
{
//   m_parity =-1;
//   m_byte = 0;
//   m_stop = -1;
//   m_rts = -1;
   m_baud = 0;
   m_protocol = -1;
   m_ipPort = -1;
   m_SerialControls = FALSE;
}

CSimCmdLine::~CSimCmdLine()
{

}


void CSimCmdLine::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
CString paramUp(lpszParam);
CString dcbTok[] = {
      "BAUD",
      "PARITY",
      "DATA",
      "STOP",
      "RTS",
      ""
   };

   paramUp.MakeUpper(); // now check for protocol selection
   if (0==strncmp("MODTCP", paramUp, 6))
   {
      m_protocol = PROTOCOL_SELMODETH;
      sscanf_s(paramUp.Mid(7), "%d", &m_ipPort);
   }
   if (0==strcmp("MODRTU", paramUp))
   {
      m_protocol = PROTOCOL_SELMOD232;
   }
   if (0==strcmp("ABSLAVE", paramUp))
   {
      m_protocol = PROTOCOL_SELAB232;
   }
   if (0==strcmp("ABMASTER", paramUp))
   {
      m_protocol = PROTOCOL_SELABMASTER232;
   }

   // check for port name
   if (0==strncmp("PORT:", paramUp,5))
   {
      m_portName = paramUp.Mid(5);
      m_DCBParams += m_portName;
      m_DCBParams += ": ";
   }

   // check commDCB parameters 'baud=1200 parity=N data=8 stop=1 ' or '96,n,8,1,p' style
   bool dcbAdd = FALSE;
   int index(0);
   while (dcbTok[index].GetLength())
   {
      if (0==strncmp(dcbTok[index], paramUp ,dcbTok[index].GetLength()) )
         dcbAdd = TRUE;
      index++;
   }
   if (0<= paramUp.Find(',')) // the ,,,,p style
       dcbAdd = TRUE;

   if (dcbAdd)
   {
      m_SerialControls = TRUE;
      m_DCBParams += lpszParam; 
      m_DCBParams += " ";
   }

   if ((bLast) && m_SerialControls)
   { // copy all DCB results into the parameters

      ASSERT(   m_baud != 0); // user failed to call SetPortParams()
      m_dcbBlock.BaudRate     = m_baud;
      m_dcbBlock.ByteSize     = (BYTE)m_byte;
      m_dcbBlock.Parity       = (BYTE)m_parity;
      m_dcbBlock.StopBits     = (BYTE)m_stop;
      m_dcbBlock.fRtsControl  = m_rts;
      if (!BuildCommDCB(m_DCBParams, &m_dcbBlock))
      {
         // parsing error!
         AfxMessageBox("Error: commandline MODE parameters were invalid");
         m_SerialControls = FALSE;
      }
      m_baud   = m_dcbBlock.BaudRate;
      m_byte   = m_dcbBlock.ByteSize;
      m_parity = m_dcbBlock.Parity;
      m_stop   = m_dcbBlock.StopBits;
      m_rts    = m_dcbBlock.fRtsControl;
   }
}

bool CSimCmdLine::GetPortSettings(char aPortName[MAX_COMPORT_NAME],
                                  DWORD &aBaudRate, 
                                  DWORD &aByteSize, 
                                  DWORD &aParity, 
                                  DWORD & aStop, 
                                  DWORD &aRTS)
{
   if (m_portName.GetLength() )
   {
      strncpy_s(aPortName , MAX_COMPORT_NAME, m_portName, MAX_COMPORT_NAME);
      aBaudRate = m_baud;
      aByteSize = m_byte;
      aParity = m_parity;
      aStop    = m_stop;
      aRTS     = m_rts;
      return(TRUE);
   }
   return(FALSE);
}

bool CSimCmdLine::GetProtocol(DWORD &aProtocol)
{
   if (m_protocol != -1)
   {
      aProtocol = m_protocol;
      return(TRUE);
   }
   return(FALSE);
}


bool CSimCmdLine::GetIPSettings(DWORD &aPort)
{
   if (m_ipPort != -1)
   {
      aPort = m_ipPort;
      return(TRUE);
   }
   return(FALSE);
}

void CSimCmdLine::SetPortParams (  const DWORD &aBaudRate, 
                         const DWORD &aByteSize, 
                         const DWORD &aParity, 
                         const DWORD & aStop, 
                         const DWORD &aRTS)
{
   m_baud =  aBaudRate;
   m_byte =  aByteSize;
   m_parity = aParity  ;
   m_stop =   aStop    ;
   m_rts =    aRTS     ;
}