--------------------------------------------------------------------------------
COMPILE:
--------------------------------------------------------------------------------

Microsoft Visual C++ 6.0


--------------------------------------------------------------------------------
BEFORE CONVERSION:
--------------------------------------------------------------------------------

1. enviroment variables should be set
   
   To set:
   My Computer -> Properties -> Advanced -> enviroment variables -> User variable... (e.g.):
 
     ssDir  = C:\VSS\MyVssDatabaseDir
     ssUser = Admin
     ssPwd  = *******

   Use Microsoft Visual SourceSafe 6.0 -> help -> search for more info


2. tmp folder should be empty

3. Check your date time format.

     Control Panel -> Regional and Language Options -> Regional Options -> Customize ->
      Date -> Short date format

       'yyyy-MM-dd'
         or
       'yyyy.MM.dd'

     Control Panel -> Regional and Language Options -> Regional Options -> Customize ->
      Time -> Time format

       'HH:mm:ss'  (24 hour NO AM/PM symbol!!!)



--------------------------------------------------------------------------------
KNOWN BUGS:
--------------------------------------------------------------------------------

#1: Sometimes file history is lost

    CAUSE:
      VSS command "ss Dir" gives unexpected output
      See file bak/D_Step1_VssDir.txt as example, find
      "AppInit.cpp         Tomas         09.10.28 11:55"
      Such output is not supported (yet)


#2: