## Overview
Back in 1996, I wrote this (**zLibDll**) customized version of the original zLib library (version 1.1.3), which is copyright (c) 1995-1998 by Jean-loup Gailly and Mark Adler.

The purpose of the customization project was to use the original zLib as a base library to compress various input files into a single output file, and then handle the extraction in a controlled manner.

The basic idea was to add all the input files to a single .gz file. Not a proprietary format containing multiple loose .gz files, but rather a single .gz file to which the compressions of the input files could be appended, as envisioned by the .gz format specifications (RFC 1952).

To manage the output .gz file as a container, a proprietary header (GZW) was designed to be inserted between one compressed file and the next, allowing for navigation and reading within the file. For this purpose, the zLib functions were surgically modified to inject the GZW header at the exact point between the writing of the original GZ header and the start of the compressed data stream.

In short, if a .gzw file (effectively an inflated .gz) is used as an object container, a mechanism (the injected GZW header) must be used to navigate through the file, skipping from one object to another.

The final result (the .gzw output file) was therefore a .gz file containing one or more compressed files inside it, with all the files appended after creating the first one, and all equipped with an additional header (the GZW header), used by the interface to browse and read its contents.

Additionally, aside from the header modifications, the whole zLib library code was reconfigured to be compiled as a Windows DLL using the Visual C++ 4.2 (keep in mind the date: 1996).

All the modifications to the original zLib source code were enclosed by the "//LPI" tag.

## Project dependencies
Source files that are not part of the core **zLibDll** project but are used by it as external dependencies can be found in the  **Include** repository. Therefore, to compile this project, you need to download the following components:

-   [zLibDll](https://github.com/lpierge/zLibDll)  — this project
-   [Include](https://github.com/lpierge/Include)  — Shared header (.h) files

## Implementation notes
Back when the project was born (1996), x86 was the only available mode. Modern versions of Windows all support x64, but I have absolutely no intention of going through the hassle of revising the entire codebase to recompile it as x64 (and even less to add Unicode support!). Especially since using x64 is not necessary at all to handle files larger than 4 GB (which was the main limitation of the old code).

The most significant change I made to the old codebase for the GZW support was precisely adding support for files bigger than 4 GB, and this did _**not**_ require switching to x64, because the 64-bit CRT functions family (_fseeki64, _ftelli64, _lseeki64, _open, etc.) is also available in the x86 (32-bit) environment (see the notes in the gzwhdr.h header for details).

The only real issue is that the two modes (x86 and x64) cannot "play together". As a result, the current DLL with support for GZW files larger than 4 GB can only be used by programs compiled in x86 mode.

A sample project using the **zLibDll** is available in the [dtcopy](https://github.com/lpierge/dtcopy) repository.

**Important note on project structure:**

The Visual Studio project for **zLibDll** is hardcoded to search for dependencies using absolute paths starting from the root of a virtual L: drive. The expected directory structure is as follows:

```text
L:\
  |-- Include\
  |-- zLibDll\
  |-- Lib\
```

The L:\Lib directory must be created manually because it is the directory where the output DLL will be copied and where projects from other repositories look for libraries and DLLs to compile and link.

Instead of changing the Visual Studio settings in the project file, I recommend mapping a local folder to a virtual L: drive with the Windows SUBST command:
- Create a directory on your local drive, for example `C:\DEV`.
- Download and extract all the repositories inside that directory.
- Open the Windows Command Prompt (press `Win + R` to open the Run dialog, type `cmd.exe` and press `Enter`) and from the Console run the following command: `SUBST L: C:\DEV`

Luca P.
