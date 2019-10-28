# BmmP
A small program to load .bmp file and output to console and file.
  
compile with gcc.  
compile as "bmp" file  
  
Usage:  
&ensp;&ensp; bmp &lt;options&gt; &lt;files&gt;  
  
Options:  
&ensp;&ensp;-z: don't skip 00 at every end of the rows (if haven't)  
&ensp;&ensp;-h: show help  
  
And there is a bug. It may lost the first row and output only Row 2th~End.
