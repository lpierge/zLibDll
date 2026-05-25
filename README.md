Back in 1996, I wrote this (zLibdll) customized version of the original zLib library (version 1.1.3), which is copyright (C) 1995-1998 by Jean-loup Gailly and Mark Adler .

The purpose of the customization project was to use the original zLib as a base library to compress various input files into a single output file, and then handle the extraction in a controlled manner.

The basic idea was to add all the input files to a single .gz file. Not a proprietary format containing multiple loose .gz files, but rather a single .gz file to which the compressions of the input files could be appended, as envisioned by the .gz format specifications (RFC 1952).

To manage the output .gz file as a container, a proprietary header (GZW) was designed to be inserted between one compressed file and the next, allowing for navigation and reading within the file. For this purpose, the zLib functions were surgically modified to inject the GZW header at the exact point between the writing of the original GZ header and the start of the compressed data stream.

In short, if a .gzw file (effectively an inflated .gz) is used as an object container, a mechanism (the injected GZW header) must be used to navigate through the file, skipping from one object to another.

The final result (the .gzw output file) was therefore a .gz file containing one or more compressed files inside it, with all the files appended after creating the first one, and all equipped with an additional header (the GZW header), used by the interface to browse and read its contents.

Additionally, aside from the header modifications, the whole zLib library code was reconfigured to be compiled as a Windows DLL using the Visual C++ 4.2 (keep in mind the date: 1996).

All the modifications to the original zLib source code were enclosed by the "//LPI" tag.

**Important note on project structure:**
The Visual Studio project for zLibDll is hardcoded to search for dependencies using absolute paths starting from the root of a virtual L: drive. The expected directory structure is as follows:

```text
L:\
  |-- Include\
  |-- zLibDll\
```
   
If you want to compile the zLibDll project without reconfiguring the Visual Studio settings, you can map a local folder to a virtual L: drive using the Windows SUBST command:
Create a directory on your local drive (for example, C:\DEV).
Download and extract all the required repositories inside that directory.
Open the Windows Command Prompt (cmd) and run the following command:
`SUBST L: C:\DEV`
Note: to remove the virtual drive you can run `SUBST L: /d`.

Luca P.
