/*

	Ajay Bechara -  1741054
	Akshay Gopani - 1741063

*/

/*

	How to run code
	Compile : g++ main.cpp -pthread
	run : a

*/

#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strsafe.h>
#include<iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<pthread.h> 

#define BUFSIZE 1024
using namespace std;

TCHAR buffer[BUFSIZE];

void DisplayErrorBox(LPTSTR lpszFunction);
int listfilesanddirectory();
void createmptyfile(string name);
void diskspaceinfo();
void pwd();
void mkdir(string input);
void cd(string input);
void rm(string filename);
void rmdir(string dirname);
void cp(string existfile,string newfile);
void pwdSilent();
int getFileList(TCHAR *path,TCHAR *target);
void readFile(string name);

#pragma comment(lib, "User32.lib");
#define BUFFERSIZE 4096

void getinput()
{
    string S, T;
    string commandsT[]={"touch","pwd","ls","mkdir","cd","rm","cp","rmdir","diskspace","help","countFiles","cat","exit"};
    int commandCount = sizeof(commandsT)/sizeof(commandsT[0]);
    vector<string> commands(commandsT , commandsT+commandCount );

    cout << "Available Commands : \n";
    for(int i=0 ; i<commands.size() ; i++){
        cout << commands[i] << "  ..  ";
    }

    cout << "\n\n";

    while(1){
	pwdSilent();
        cout<< "\n\n" << buffer << " >>> ";
    
        getline(cin, S);
        stringstream X(S);
        
        int cnt=0;
        vector<string> str;
        
        while (getline(X, T, ' ')) {
            cnt++;
            str.push_back(T);
        }
        
        int choice=0;

        for(int i=0; i<commands.size(); i++)
        {
            if((str[0].compare(commands[i]))==0)
            {
                choice=i+1;
                break;
            }
        }
        
        switch(choice)
        {
            case 1:  // touch
            createmptyfile(str[1]);
            break;
            
            case 2:  // pwd
	    
            pwd();
            break;
            
            case 3:
            listfilesanddirectory();
            break;
            
            case 4:
            cout<<str[1]<<endl;
            mkdir(str[1]);
            break;
            
            case 5:
            cout<<str[1]<<endl;
            cd(str[1]);
            break;
            
            case 6:
            cout<<str[1]<<endl;
            rm(str[1]);
            break;
            
            case 7:
            cout<<str[1]<<endl;
            cp(str[1],str[2]);
            break;
            
            case 8:
            cout<<str[1]<<endl;
            rmdir(str[1]);
            break;
            
            case 9:
            diskspaceinfo();
            break;

	    case 10:
	    cout << "\nAvailable Commands : \n";
            for(int i=0 ; i<commands.size() ; i++){
                cout << commands[i] << "  ..  ";
            }
            cout << "\n\n";
            break;
            
	    case 11:
            
		TCHAR path[1024];
		TCHAR filename[1024];
		memset(path,0,1024);
		memset(filename , 0 , 1024);
		
		strcpy(path, str[1].c_str());
		strcpy( filename , str[2].c_str());

		cout << "Count = " << getFileList(path,filename) << endl;
	    
	    break;

	    case 12:
		readFile(str[1]);
	    break;
	
	    case 13:
		exit(0);
	    break;

            default:
            cout << "Command " << str[0] << " Not Found" << endl;
            break;
	    cout << endl << endl;
        }
    }
}


int getFileList(TCHAR *path,TCHAR *target){
    
   DWORD count = 0;
   WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   
   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.

   StringCchLength(path, MAX_PATH, &length_of_arg);

   if (length_of_arg > (MAX_PATH - 3))
   {
      _tprintf(TEXT("\nDirectory path is too long.\n"));
      return (-1);
   }

  // _tprintf(TEXT("\nTarget directory is %s\n\n"), path);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   StringCchCopy(szDir, MAX_PATH, path);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
      	_tprintf(TEXT("Not Able to Find First File"));
      	return -1;
   }
   
   // List all the files in the directory with some info about them.
	TCHAR dot[5] = ".";
	TCHAR dDot[5] = "..";
   do
   {
      if( (strcmp(dot,ffd.cFileName) == 0) ||  (strcmp(dDot,ffd.cFileName) == 0)  ){
	// Not want to print
      }else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){
 //       _tprintf(TEXT("'%s'  "), ffd.cFileName);
	 TCHAR buf[255];
	memset(buf,0,255);
    
    StringCchCatNA(
      buf,
      255,
      path,
      strlen(path)
    );
    
    StringCchCatNA(
      buf,
      255,
      "/",
      strlen("/")
    );
    
    StringCchCatNA(
      buf,
      255,
      ffd.cFileName,
      strlen(ffd.cFileName)
    );
	 
	count += getFileList(buf,target);

      }else{
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
         if(  strcmp(target,ffd.cFileName) == 0 )
            count++;
      }
   }
   while ( FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES)
   {
      _tprintf(TEXT("Not Able to Find Files"));
   }

   FindClose(hFind);
   
   return count;
}


void createmptyfile(string name)
{
     HANDLE hFile     = INVALID_HANDLE_VALUE;
    HANDLE hTempFile = INVALID_HANDLE_VALUE;

    BOOL fSuccess  = FALSE;
    DWORD dwRetVal = 0;
    UINT uRetVal   = 0;

    DWORD dwBytesRead    = 0;
    DWORD dwBytesWritten = 0;

    TCHAR szTempFileName[MAX_PATH];
    TCHAR lpTempPathBuffer[MAX_PATH];
    char  chBuffer[BUFSIZE];

    LPCTSTR errMsg;

      TCHAR* str=new TCHAR[name.size()];
      str[name.size()]=0;
      copy(name.begin(),name.end(),str);

    cout<<str<<endl;
    if(strlen(str)>255)
    {
        cout<<"File name too long"<<endl;
    }
    cout<<str<<endl;
    //  Opens the existing file.

    hTempFile = CreateFile(str, // file name
                           GENERIC_WRITE | GENERIC_READ,        // open for write
                           0,                    // do not share
                           NULL,                 // default security
                           CREATE_ALWAYS,        // overwrite existing
                           FILE_ATTRIBUTE_NORMAL,// normal file
                           NULL);

    if (hTempFile == INVALID_HANDLE_VALUE)
    {
        cout<<"error"<<endl;
    }
    else
    {
        CloseHandle(hTempFile);
    }
}

void pwd()
{
    DWORD retval;
    bool success;
    
    retval = GetCurrentDirectory(BUFSIZE,buffer);
    
    if (retval == 0)
    {
        // Handle an error condition.
        printf ("GetFullPathName failed (%d)\n", GetLastError());
    }
    else
    {
/*		string s(buffer);
		_tprintf(TEXT("The full path name is:  %s\n"), s);
             	string lastDirectory;
   		string token;
   		istringstream tokenStream(s);
		char delimiter = '\\';

		while (getline(tokenStream, token, delimiter))
   		{
			lastDirectory = token;
			_tprintf(TEXT("The full path name is:  %s\n"), token);
   		}
*/
        _tprintf(TEXT("The full path name is:  %s\n"), buffer);
    }
}

void pwdSilent()
{
    DWORD retval;
    bool success;
    
   // char buffer[BUFSIZE];
    retval = GetCurrentDirectory(BUFSIZE,buffer);
    
}

void mkdir(string input)
{
    BOOL   success;
    char* c= new TCHAR[input.length() + 1];
    strcpy(c,input.c_str());
    
    //cout<<c<<endl;
    success=CreateDirectoryA(c,NULL);
    //cout<<success<<endl;
    
    if(success)
    {
        cout<<"directory created successfully\n"<<endl;
    }
    else
    {
        cout<<"failed to create directory"<<endl;
    }
}
void cd(string input)
{
    BOOL   success;
    char* c= new TCHAR[input.length() + 1];
    strcpy(c,input.c_str());
    
    //cout<<c<<endl;
    success=SetCurrentDirectory(c);
    
    // pwd();
    //cout<<success<<endl;
    if(success)
    {
        cout<<"directory changed successfully\n"<<endl;
    }
    else
    {
        cout<<"failed to change directory"<<endl;
    }
    
    // char *args[]={"C:\Users\shree\Desktop\os\Shell\getcd",NULL};
    //execvp(args[0],args);
}

int listfilesanddirectory()
{
    pwdSilent();
    
    string input=buffer;
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError=0;
    
    TCHAR* argv=new TCHAR[input.size()];
    argv[input.size()]=0;
    copy(input.begin(),input.end(),argv);
    
    StringCchLength(argv, MAX_PATH, &length_of_arg);
    
    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }
    
    //   _tprintf(TEXT("\nTarget directory is %s\n\n"),argv);
    
    
    StringCchCopy(szDir, MAX_PATH, argv);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    
    // Find the first file in the directory.
    
    hFind = FindFirstFile(szDir, &ffd);
    
    if (INVALID_HANDLE_VALUE == hFind)
    {
        _tprintf(TEXT("Not Able to Find First File"));
        return dwError;
    }
    
    // List all the files in the directory with some info about them.
    TCHAR dot[5] = ".";
    TCHAR dDot[5] = "..";
    int totalfile=0;
    int directory = 0;
    int files = 0;
    do
    {
        if( (strcmp(dot,ffd.cFileName) == 0) ||  (strcmp(dDot,ffd.cFileName) == 0)  ){
        // Not want to print
        }else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
             _tprintf(TEXT("'%s'  "), ffd.cFileName);
		directory++;
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            //   _tprintf(TEXT("%s   %ld bytes  "), ffd.cFileName, filesize.QuadPart);
            _tprintf(TEXT("%s   "), ffd.cFileName);
		files++;
        }
        totalfile++;
    }
    
    while (FindNextFile(hFind, &ffd) != 0);
    cout<<" "<<endl;
    
    cout<<"\nTotal Files : " << files << " & " << "Total Directories " << directory << endl;
    dwError = GetLastError();
    
    if (dwError != ERROR_NO_MORE_FILES)
    {
        _tprintf(TEXT("Not Able to Find Files"));
    }
    
    FindClose(hFind);
    return dwError;
}

void filesize(string filename)
{
    TCHAR* argv=new TCHAR[filename.size()];
    argv[filename.size()]=0;
    
    copy(filename.begin(),filename.end(),argv);
    HANDLE hFile;
    DWORD  dwBytesRead = 0;
    
    hFile = CreateFile(argv,               // file to open
                   GENERIC_READ,          // open for reading
                   FILE_SHARE_READ,       // share for reading
                   NULL,                  // default security
                   OPEN_EXISTING,         // existing file only
                   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
                   NULL);                 // no attr. template
                   
     DWORD filesize=GetFileSize(hFile,NULL);
     cout<<filesize<<endl;
}
void rm(string filename)
{
    BOOL   success;
    char* c= new TCHAR[filename.length() + 1];
    strcpy(c,filename.c_str());
    
    success=DeleteFileA(c);
    
    if(success==1)
    {
        cout<<"filedeleted"<<endl;
    }
    else
        cout<<"-------error---------"<<endl;
    
}

void cp(string existfile,string newfile)
{
    char* e= new TCHAR[existfile.length() + 1];
    strcpy(e,existfile.c_str());
    
    char* n= new TCHAR[newfile.length() + 1];
    strcpy(n,newfile.c_str());
    BOOL   success;
    
    success=CopyFile(e,n,FALSE);     //if new file is already exist then it will overwrite if FALSE last param
    if(success==1)
    {
        cout<<"filecopied"<<endl;
    }
    else
        cout<<"-------error---------"<<endl;
}

void rmdir(string dirname)
{
    BOOL   success;
    char* dir= new TCHAR[dirname.length() + 1];
    strcpy(dir,dirname.c_str());
    
    success=RemoveDirectoryA(dir);
    
    if(success==1)
    {
        cout<<"directory deleted"<<endl;
    }
    else
        cout<<"-------error---------"<<endl;
}

void diskspaceinfo()
{

    BOOL success;
    
    // LPCSTR  lpRootPathName1="C:";
    LPCSTR  lpRootPathName1="C:";
    LPDWORD lpSectorsPerCluster;
    LPDWORD lpBytesPerSector;
    LPDWORD lpNumberOfFreeClusters;
    LPDWORD lpTotalNumberOfClusters;

    cout<<lpRootPathName1<<endl;
    
    // cout<<lpRootPathName2<<endl;
    
    success=GetDiskFreeSpaceA(
    lpRootPathName1,
    lpSectorsPerCluster,
    lpBytesPerSector,
    lpNumberOfFreeClusters,
    lpTotalNumberOfClusters
    );
    
    if(success)
    {
        cout<<"dsdsdsd"<<endl;
        cout<<lpRootPathName1<<endl;
        cout<<lpSectorsPerCluster<<endl;
        cout<<lpBytesPerSector<<endl;
        cout<<lpNumberOfFreeClusters<<endl;
        cout<<lpTotalNumberOfClusters<<endl;
    }
    
    /*  success=GetDiskFreeSpaceA(
    lpRootPathName1,
    lpSectorsPerCluster,
    lpBytesPerSector,
    lpNumberOfFreeClusters,
    lpTotalNumberOfClusters
    );
    if(success)
    {
    cout<<lpRootPathName2<<endl;
    cout<<lpSectorsPerCluster<<endl;
    cout<<lpBytesPerSector<<endl;
    cout<<lpNumberOfFreeClusters<<endl;
    cout<<lpTotalNumberOfClusters<<endl;
    }*/
}

void readFile(string name){

    HANDLE hFile     = INVALID_HANDLE_VALUE;
    HANDLE hTempFile = INVALID_HANDLE_VALUE;
    BOOL fSuccess  = FALSE;
    DWORD   lpNumberOfBytesRead=0;
    OVERLAPPED ol = {0};

    TCHAR szTempFileName[MAX_PATH];
    TCHAR lpTempPathBuffer[MAX_PATH];
    char  chbuffer[BUFFERSIZE];

    TCHAR* str=new TCHAR[name.size()];
    str[name.size()]=0;
    copy(name.begin(),name.end(),str);

    if(strlen(str)>255)
    {
        cout<<"File name too long"<<endl;
    }

    hTempFile = CreateFile(str, // file name
                           GENERIC_READ,        // open for write
                           0,                    // do not share
                           NULL,                 // default security
                           OPEN_EXISTING,        // overwrite existing
                           FILE_ATTRIBUTE_NORMAL,// normal file
                           NULL);

    if (hTempFile == INVALID_HANDLE_VALUE)
    {
        cout<<"error"<<endl;
    }
    else
    {
       while(1)
        {
	   memset(chbuffer,0,BUFFERSIZE);

           fSuccess= ReadFile(hTempFile,chbuffer,BUFFERSIZE,&lpNumberOfBytesRead,NULL);

           if(fSuccess && lpNumberOfBytesRead==0)
           {
               break;
           }
           else
           {
               cout<<chbuffer<<endl;
           }

           // memset(chbuffer,0,sizeof(chbuffer));
        }
          CloseHandle(hTempFile);
    }

}

int  main()
{
    getinput();
    return 0;
}