#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <stdio.h>
#include <winbase.h>
#include <windows.h>
#include <Tlhelp32.h>

#define DEF_BUF_SIZE 1024 

 // ���ڴ洢ע��ģ��DLL��·��ȫ�� 
 //char szDllPath[DEF_BUF_SIZE] = {0} ; 
/* typedef void (__stdcall *PAPCFUNC)(unsigned long);
 typedef HANDLE (__stdcall *OPENTHREAD) (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);  
OPENTHREAD OpenThread = (OPENTHREAD)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenThread");  
 typedef long(__stdcall *pQueueUserAPC)( PAPCFUNC pfnAPC, HANDLE hThread, long* dwData ); 
pQueueUserAPC QueueUserAPC =(pQueueUserAPC)GetProcAddress(LoadLibrary("kernel32.dll"),"QueueUserAPC");  */


void Logn(char* x, long y)
{
    FILE *fp;
    fp = fopen("d:\\hooknum.txt", "ab+");
    if (fp != NULL)
    {
        char log[255];
        wsprintf(log, "%s: %d\r\n", x, y);
        fprintf(fp, "%s", log);
        fclose(fp);
    }
}

void Logwn(WCHAR* x, long y)
{
    FILE *fp;
    fp = fopen("d:\\hooknum.txt", "ab+");
    if (fp != NULL)
    {
        char log[255];
        wsprintf(log, "%ws: %d\r\n", x, y);
        fprintf(fp, "%ws", log);
        fclose(fp);
    }
}

void Logss(char* x, char *  y)
{
    FILE *fp;
    fp = fopen("d:\\hooknum.txt", "ab+");
    if (fp != NULL)
    {
        char log[255];
        wsprintf(log, "%s %s \n", x, y);
        fprintf(fp, "%s", log);
        fclose(fp);
    }
}

 // ʹ��APC������ָ��ID�Ľ���ע��ģ�� 

 int InjectModuleToProcess ( HANDLE hProcess ,HANDLE dwProcessId,char *szDllPath) 
 { 
     size_t   dwRet = 0 ; 
     BOOL    bStatus = FALSE ; 
     LPVOID  lpData = NULL ; 
     size_t    uLen = strlen(szDllPath) + 1; 

         // ����ռ� 
         lpData = VirtualAllocEx ( hProcess, NULL, uLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE ) ; 
         if ( lpData ) 
         { 
             // д����Ҫע���ģ��·��ȫ�� 
             bStatus = WriteProcessMemory ( hProcess, lpData, szDllPath, uLen, &dwRet ) ; 
         } 
         CloseHandle ( hProcess ) ; 

  
     // �����߳̿��� 
     THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ; 
     HANDLE hThreadSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPTHREAD, 0 ) ; 
     if ( hThreadSnap == INVALID_HANDLE_VALUE )  
         return 1 ;  

     bStatus = 2 ; 
     // ö�������߳� 
     if ( Thread32First ( hThreadSnap, &te32 ) ) 
     { 
         do{ 
             // �ж��Ƿ�Ŀ������е��߳� 

             if ( te32.th32OwnerProcessID == (unsigned long)dwProcessId ) 
             { 
                 // ���߳� www.2cto.com  
                 HANDLE hThread = OpenThread ( THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID ) ; 
                 if ( hThread ) 
                 { 
                     // ��ָ���߳�����APC 
                     DWORD dwRet = QueueUserAPC ( (PAPCFUNC)LoadLibraryA, hThread,(ULONG_PTR) lpData ) ; 
                     if ( dwRet > 0 ) 
                         bStatus = 0 ; 
                     CloseHandle ( hThread ) ; 
                 } 

             }  
  
         }while ( Thread32Next ( hThreadSnap, &te32 ) ) ; 
     } 
  
     CloseHandle ( hThreadSnap ) ; 
     return bStatus; 
 } 


 
 long SuspendProcess(DWORD dwProcessId,long selftid)
 {

     BOOL    bStatus = FALSE ; 
  
     // �����߳̿��� 
     THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ; 
     HANDLE hThreadSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPTHREAD, 0 ) ; 
     if ( hThreadSnap == INVALID_HANDLE_VALUE )  
         return 1 ;  

     bStatus = 0 ; 
     // ö�������߳� 
     if ( Thread32First ( hThreadSnap, &te32 ) ) 
     { 
         do{ 
             // �ж��Ƿ�Ŀ������е��߳� 

             if ( te32.th32OwnerProcessID == (unsigned long)dwProcessId ) 
             { 
                 // ���߳� www.2cto.com  
				 if (te32.th32ThreadID!=selftid)
				 {
					 HANDLE hThread = OpenThread ( THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID ) ; 
					 if ( hThread  ) 
					 { 
						 SuspendThread(hThread);
						 CloseHandle ( hThread ) ; 
					 
					 } 
					 else
					 {
						 bStatus=23;
					 }
				 }

             }  
  
         }while ( Thread32Next ( hThreadSnap, &te32 ) ) ; 
     } 
	 else
	 {
			bStatus=24;
	 }
  
     CloseHandle ( hThreadSnap ) ; 
	 return bStatus;
 }

 long ResumeProcess(DWORD dwProcessId,long selftid)
 {

     BOOL    bStatus = FALSE ; 
  
     // �����߳̿��� 
     THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ; 
     HANDLE hThreadSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPTHREAD, 0 ) ; 
     if ( hThreadSnap == INVALID_HANDLE_VALUE )  
         return 1 ;  

     bStatus = 0 ; 
     // ö�������߳� 
     if ( Thread32First ( hThreadSnap, &te32 ) ) 
     { 
         do{ 
             // �ж��Ƿ�Ŀ������е��߳� 

             if ( te32.th32OwnerProcessID == (unsigned long)dwProcessId ) 
             { 
                 // ���߳� www.2cto.com  
				 if (te32.th32ThreadID!=selftid)
				 {
					 HANDLE hThread = OpenThread ( THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID ) ; 
					 if ( hThread ) 
					 { 
						 ResumeThread(hThread);
						 CloseHandle ( hThread ) ; 
					 
					 } 
					 else
					 {
						 bStatus=23;
					 }
				 }
             }  
  
         }while ( Thread32Next ( hThreadSnap, &te32 ) ) ; 
     } 
	 else
	 {
			bStatus=24;
	 }
  
     CloseHandle ( hThreadSnap ) ; 
	 return bStatus;
 }

 BOOL InjectModuleToProcessById ( DWORD dwProcessId ,char *szDllPath) 
 { 
     size_t   dwRet = 0 ; 
     BOOL    bStatus = FALSE ; 
     LPVOID  lpData = NULL ; 
     size_t    uLen = strlen(szDllPath) + 1; 
     // ��Ŀ����� 
     HANDLE hProcess = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, dwProcessId ) ; 
     if ( hProcess ) 
     { 
         // ����ռ� 
         lpData = VirtualAllocEx ( hProcess, NULL, uLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE ) ; 
         if ( lpData ) 
         { 
             // д����Ҫע���ģ��·��ȫ�� 
             bStatus = WriteProcessMemory ( hProcess, lpData, szDllPath, uLen, &dwRet ) ; 
         } 
         CloseHandle ( hProcess ) ; 
     } 
  
     if ( bStatus == FALSE ) 
         return FALSE ; 
  
     // �����߳̿��� 
     THREADENTRY32 te32 = { sizeof(THREADENTRY32) } ; 
     HANDLE hThreadSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPTHREAD, 0 ) ; 
     if ( hThreadSnap == INVALID_HANDLE_VALUE )  
         return FALSE ;  
  
     bStatus = FALSE ; 
     // ö�������߳� 
     if ( Thread32First ( hThreadSnap, &te32 ) ) 
     { 
         do{ 
             // �ж��Ƿ�Ŀ������е��߳� 
             if ( te32.th32OwnerProcessID == dwProcessId ) 
             { 
                 // ���߳� www.2cto.com  
                 HANDLE hThread = OpenThread ( THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID ) ; 
                 if ( hThread ) 
                 { 
                     // ��ָ���߳�����APC 
                     DWORD dwRet = QueueUserAPC ( (PAPCFUNC)LoadLibraryA, hThread, (ULONG_PTR)lpData ) ; 
                     if ( dwRet > 0 ) 
                         bStatus = TRUE ; 
                     CloseHandle ( hThread ) ; 
                 } 
             }  
  
         }while ( Thread32Next ( hThreadSnap, &te32 ) ) ; 
     } 
  
     CloseHandle ( hThreadSnap ) ; 
     return bStatus; 
 } 




 bool WINAPI  __stdcall InjectDll(DWORD processID,LPCTSTR Path )   
{   
    HANDLE hThread;   
    char   szLibPath [MAX_PATH];   
    void*  pLibRemote = 0;  // the address (in the remote process) where    
                            // szLibPath will be copied to;    
    DWORD  hLibModule = 0;  // base adress of loaded module (==HMODULE);    
   HANDLE  hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processID) ;

    HMODULE hKernel32 = ::GetModuleHandle("Kernel32");//if Kernel32 has been map into the address    
   
    // Get full path of "LibSpy.dll"    
    //if( !GetModuleFileName(NULL,szLibPath,MAX_PATH) )//get the current file name    
      //  return false;   
    strcpy( szLibPath,Path );   
  // MessageBox(NULL,szLibPath,NULL,64);
   
    // 1. Allocate memory in the remote process for szLibPath    
    // 2. Write szLibPath to the allocated memory    
    pLibRemote = ::VirtualAllocEx( hProcess, NULL, sizeof(szLibPath), MEM_COMMIT, PAGE_READWRITE );//����һ�� ���̷��ɿռ�    
    if( pLibRemote == NULL )   
        return false;   
    ::WriteProcessMemory(hProcess, pLibRemote, (void*)szLibPath,sizeof(szLibPath),NULL);//����һ�� ���� �ռ� д���Լ��Ľ���    
   
   
    // Load "LibSpy.dll" into the remote process     
    // (via CreateRemoteThread & LoadLibrary)    

	hThread = ::CreateRemoteThread( hProcess, NULL, 0,     
                    (LPTHREAD_START_ROUTINE) ::GetProcAddress(hKernel32,"LoadLibraryA"),    
                    pLibRemote, 0, NULL );   

	if( hThread == NULL )   
        goto JUMP;   
    ResumeThread(hThread);
    if(::WaitForSingleObject( hThread, 5000 )==WAIT_TIMEOUT)
	{
		CloseHandle( hThread );
		return 0;
	}

    // Get handle of loaded module    
    ::GetExitCodeThread( hThread, &hLibModule );   
    ::CloseHandle( hThread );   
   
JUMP:      
    ::VirtualFreeEx( hProcess, pLibRemote, sizeof(szLibPath), MEM_RELEASE );   
    if( hLibModule == NULL )   
        return false;   
       
   
    // Unload "LibSpy.dll" from the remote process     
    // (via CreateRemoteThread & FreeLibrary)    
 /*   hThread = ::CreateRemoteThread( hProcess,   
                NULL, 0,   
                (LPTHREAD_START_ROUTINE) ::GetProcAddress(hKernel32,"FreeLibrary"),   
                (void*)hLibModule,   
                 0, NULL );   
    if( hThread == NULL )   // failed to unload    
        return false;   
   
    if(::WaitForSingleObject( hThread, 5000 )==WAIT_TIMEOUT)
	{
		CloseHandle( hThread );
		return 0;
	}
    ::GetExitCodeThread( hThread, &hLibModule );   
    ::CloseHandle( hThread );   */
    CloseHandle(hProcess);
    // return value of remote FreeLibrary (=nonzero on success)    
    return 1;   
}   