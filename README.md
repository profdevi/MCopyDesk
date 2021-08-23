# MCopyDesk

Program copies single files from desktop folder of Windows to current directory.   
This is just some fun code for testing some Windows COM interfaces, and for me
to practice some C++ coding again.  The program is just an elaborate version
of the command 

    C:> copy %USERPROFILE%\Downloads\FileName .

You can compile the program using CMake, and the example usage is shown
below.  See the LICENSE file for license requirements.
    
-Prof Devi

-----------------------------------------------------------------------------

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



