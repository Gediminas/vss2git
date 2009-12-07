--------------------------------------------------------------------------------
INFO
--------------------------------------------------------------------------------

This console application converts Microsoft Visual SourceSafe database to GIT repository

Simple cycle "ss get" "git commit" is used.
Commits are grouped by user but split by date



--------------------------------------------------------------------------------
COMPILE
--------------------------------------------------------------------------------

Microsoft Visual C++ 6.0



--------------------------------------------------------------------------------
BEFORE CONVERSION
--------------------------------------------------------------------------------

1. environment variables should be set
   
   To set:
   My Computer -> Properties -> Advanced -> enviroment variables -> User variable... (e.g.):
 
     ssDir  = C:\VSS\MyVssDatabaseDir
     ssUser = Admin
     ssPwd  = *******

   Use Microsoft Visual SourceSafe 6.0 -> help -> search for more info


2. "tmp" folder should be empty or deleted

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
CONVERSION
--------------------------------------------------------------------------------

Conversion is devided into 4 steps:

Step 1: 

  Step1_VssDir.txt       - current VSS database tree dumped using VSS command "ss dir"


Step 2: 

  Step2_Info.txt         - file+version list; used VSS command "ss history" for every file
  Step2_SkippedPaths.txt - list of files that will be skipped by import


  Currently only *.cpp && *.h files are imported (other skipped)
  This is hardcoded in src/Functions/Processor.cpp -> Step2_CollectInfo()


Step 3:
  Step3_Grouped.txt      - data prepared for git commit actions (separated by ****)
                           grouped by user but splitter by date
                           file list contains only last VSS versions to use (intermediate skipped)


Step 4:
  Step4_Import.txt       - import log
                           
                           "ss get"     - used to get VSS file particular version
                           "git commit" - commit everything and adjust commit date


When particular step is successfully finished file Step*.txt.DONE is generated.
Such step will be skipped during next Conversion.
Delete this file to regenerate data.



IMPORTED REPOSITORY IS GENERATED IN FOLDER: tmp/Working



--------------------------------------------------------------------------------
AFTER CONVERSION
--------------------------------------------------------------------------------


Converted git repository will contain only *.cpp && *.h file. 

You should manually copy (overwrite) all files you need from
original VSS folder on top of new git repository.

Configure ".gitignore"

And commit.



--------------------------------------------------------------------------------
REPOSITORY UPDATE
--------------------------------------------------------------------------------


You can update already imported repository (if VSS database has changed during conversion).
Or import database from specified date ignoring previous history

Copy  bak/FromDate.cfg --> tmp/FromDate.cfg

Edit stored date in tmp/FromDate.cfg

Run conversion application.
Warning should be echoed.



--------------------------------------------------------------------------------
TROUBLESHOOTING
--------------------------------------------------------------------------------


1. Wrong dates in git repository after/during conversion.

   CAUSE:
     Not supported system date/time format.
     See "BEFORE CONVERSION" 3. how to change it or simply select "Lithuanian"
      in Control Panel -> Regional and Language Options -> Regional Options



--------------------------------------------------------------------------------
KNOWN BUGS
--------------------------------------------------------------------------------

#1: Sometimes file history is lost

    CAUSE:
      VSS command "ss Dir" gives unexpected output
      See file bak/D_Step1_VssDir.txt as example, find
      "AppInit.cpp         Tomas         09.10.28 11:55"
      Such output is not supported (yet)


#2: