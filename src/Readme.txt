The code is overly complicated to just copy Windows files from the Download 
folder to the current directory.  It is very easy to just issue a command such as

	C:>  copy %USERPROFILE%\Downloads\FileName 
    
where FileName is the file that you want to copy from the Downloads folder.

The code is really to test out the COM Interface access to the Folder Mappings.
Anyway, the command usage is shown below.  See License of source code.  Building
the code uses CMake.

-Prof Devi

---------------------------------------------------------------------------

   usage:  MCopyDown [-?] [<fileindex>] [<fileindex> <filename>]
           v1.0 copyright Comine.com

   Program will copy files from the downloads folder to current directory

   options:
        <fileindex>  :  File Index for copy
        <filename>   :  Renamed file

   Example #1: Show file in Downloads folder
            c:>  MCopyDown

   Example #2: Copy 4th file to current directory
            c:>  MCopyDown 3

   Example #3: Copy First file and rename it to one.jpg
            c:>  MCopyDown 0 one.jpg

