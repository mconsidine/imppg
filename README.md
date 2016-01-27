# ImPPG (Image Post-Processor)
Copyright (C) 2015, 2016 Filip Szczerek (ga.software@yahoo.com)

version 0.5 (2016-01-02)

*This program comes with ABSOLUTELY NO WARRANTY. This is free software, licensed under GNU General Public License v3 or any later version and you are welcome to redistribute it under certain conditions. See the LICENSE file for details.*


----------------------------------------

- 1\. Introduction
- 2\. User interface features
- 3\. Supported image file formats
- 4\. Image processing
 - 4\.1\. Brightness normalization
 - 4\.2\. Lucy–Richardson deconvolution
 - 4\.3\. Unsharp masking
 - 4\.4\. Tone curve
- 5\. Saving/loading settings
- 6\. Batch processing
- 7\. Image sequence alignment
  - 1\. High-contrast features stablization (phase correlation)
  - 2\. Solar limb stabilization
- 8\. Misc
- 9\. Known problems
- 10\. Downloading
- 11\. Building from source code
  - 11\.1\. Building under Linux and similar systems using GNU (or compatible) toolchain
  - 11\.2\. Building under MS Windows
  - 11\.3\. UI language
- 12\. Change log

----------------------------------------
## 1. Introduction

ImPPG (Image Post-Processor) performs common image post-processing tasks, in this order:

  - brightness levels normalization
  - non-blind Lucy–Richardson deconvolution with a Gaussian kernel
  - blurring or sharpening via unsharp masking
  - tone curve adjustment

(all the operations are optional). The processing is performed using 32-bit (single-precision) floating-point arithmetic. Settings for all the above steps can be saved to a file and used for batch processing of multiple images.

ImPPG can also align an image sequence, with possibly large and chaotic translations between images. This can be useful, for example, when preparing a solar time-lapse animation, where subsequent frames are offset due to inaccurate tracking of the telescope mount. Other possible applications are smoothing out of terrestrial landscape time-lapses or preparing raw frames (with serious image jitter) for stacking.

The alignment is performed (with sub-pixel accuracy) either via phase correlation, which automatically aligns on the most contrasty features, or by detecting and stabilizing the solar limb.


----------------------------------------
## 2. User interface features

  - The processing controls panel, initially docked on the left of the main window, can be undocked or docked on the right.

  - Both the processing controls panel and the tone curve editor can be closed and later restored via the "View" menu or toolbar buttons.

  - Changes to the L–R deconvolution, unsharp masking settings and the tone curve are applied only to the current selection within the image (bounded by an inverted-brightness or dashed frame). To process the whole image, use the `Select (and process) all` option from `Edit` menu or the corresponding toolbar button.

  - The current selection can be changed at any time by dragging the mouse with left button pressed. Changing the selection cancels all previously started processing (if any).

  - The smaller the selection, the faster the processing. For fine-tuning of L–R deconvolution’s *sigma*, it is recommended to use moderately small areas; this way moving the *sigma* slider will show the results almost instanteneously. The refreshing is slightly slower when the current zoom factor is not 100%.

  - The view can be zoomed in/out by using toolbar buttons, `View` menu items or `Ctrl+mouse wheel`.

  - The view can be scrolled by dragging with the middle mouse button.
    
----------------------------------------
## 3. Supported image file formats

Accepted input formats: BMP, JPEG, PNG, TIFF (most of bit depths and compression methods), TGA and other via the FreeImage library, FITS. Image is processed in grayscale and saved in one of the following formats: BMP 8-bit; PNG 8-bit; TIFF 8-bit, 16-bit, 32-bit floating-point (no compression or compressed with LZW or ZIP), FITS 8-bit, 16-bit or 32-bit floating-point.

Output images produces by the sequence alignment function are saved as uncompressed TIFF with number of channels and bit depth preserved (except 8-bit palettized ones; those are converted to 24-bit RGB). Input FITS files are saved as FITS with bit depth preserved.


----------------------------------------
## 4. Image processing

----------------------------------------
### 4.1. Brightness normalization

ImPPG can automatically adjust the input image’s brightness levels (before all other processing steps) to fall into the user-supplied range of `min` to `max` (specified as percentage of the full brightness; 0% = black, 100% = white). The darkest input pixels will get the `min` value, the brightest: the `max` value.

Both `min` and `max` can be less than 0% and larger than 100% (this may clip the histogram). It is also allowed for `max` to be less than `min`; this will invert brightness levels (create a negative; also possible via tone curve adjustment, see sec. 4.4).

Normalization may be useful when processing a sequence of astronomical stack images captured during variable air transparency (i.e. with varying brightness levels).

Access by:
    menu: `Settings`/`Normalize brightness levels...`

    
----------------------------------------
### 4.2. Lucy–Richardson deconvolution

ImPPG performs image sharpening via non-blind Lucy–Richardson deconvolution using Gaussian kernel. The kernel’s width is specified by Gaussian *sigma*; increasing this value makes the sharpening coarser.

Recommended number of deconvolution iterations: 30 to 70. Specify 0 to disable L–R deconvolution.

The `Prevent ringing` checkbox enables an experimental function which reduces ringing (halo) around over-exposed areas (e.g. a solar disc in a prominence image) caused by sharpening.

Access by:
    `Lucy–Richardson deconvolution` tab in the processing controls panel (on the left of the main window)

    
----------------------------------------    
### 4.3. Unsharp masking

Unsharp masking can be used for final sharpening (independently of L–R deconvolution) or blurring of the image. The *sigma* parameter specifies the Gaussian kernel’s width; the larger the value, the coarser the sharpening or blurring. `Amount` specifies the effect’s strength. Value < 1.0 blurs the image, 1.0 does nothing, value > 1.0 sharpens the image.


#### Adaptive mode

In this mode the amount varies, depending on the input (unprocessed) image brightness. For areas darker than `threshold − transition width` the amount is set to `amount min`. For areas brighter than `threshold + transition width` the amount is set to `amount max`. Between those values the amount changes smoothly from `amount min` to `amount max`.

An example situation where this mode can be useful is stretching of faint solar prominences with the tone curve. Typically this will also underline the image noise (especially if L–R was applied), as the darker image areas have small signal-to-noise ratio. By setting `amount max` to a sharpening value (>1.0) appropriate for the solar disc interior, but `amount min` to 1.0 (i.e. no sharpening) and `threshold` to fall somewhere at the disc/prominences transition, the faint prominences and background are left unsharpened. They can be even smoothed out by setting `amount min` to <1.0, while keeping rest of the disc sharpened.

Is is also allowed to set `amount min` > `amount max`.
    
Access by:
    `Unsharp masking` box in the processing controls panel (on the left of the main window)
    

----------------------------------------
### 4.4. Tone curve

The tone curve editor allows changing the input-output mapping of brightness levels. The tone curve is only applied after all the other processing steps. The histogram shown in the editor window’s background corresponds with the output (just the current selection) of previous processing steps **before** the application of the tone curve.

Access by:
    menu: `View`/`Panels`/`Tone curve` or the corresponding toolbar button

    
----------------------------------------
## 5. Saving/loading settings

All settings described in **4.1-4.4** can be saved to/restored from a file (in XML format). A settings file is needed in order to use the batch processing function.

Access by:
    menu: `File` or the corresponding toolbar buttons

    
----------------------------------------
## 6. Batch processing

Multiple images can be processed using the same settings specified by an existing settings file (see sec. 5). Output images are saved in the specified folder under names appended with `_out`.

Access by:
    menu: `File`/`Batch processing...`


----------------------------------------
## 7. Image sequence alignment    

The `Input image files` list in the `Image alignment` dialog contains the input files in the order they are to be aligned as a sequence. List items can be removed and shifted using the buttons in the upper right corner.

Enabling `Sub-pixel alignment` results in a smoother animation and less image drift. Saving of output files will be somewhat slower; sharp, 1-pixel details (if present) may get very slightly blurred.

The output files are saved under names suffixed with `_aligned`. FITS files are saved as FITS, the remaining formats: as TIFF files.


----------------------------------------
### 7.1. High-contrast features stabilization (phase correlation)
    
A general-purpose method. Attempts to keep the high-contrast features (e.g. sunspots, filaments, prominences, craters) stationary. In some cases it may be undesirable, e.g. in a multi-hour time-lapse of a sunspot nearing the solar disc’s limb; phase correlation would tend to keep the sunspot stationary, but not the limb.
    
    
----------------------------------------
### 7.2. Solar limb stabilization
    
Detects and matches the solar limb position. The more of the limb is visible in input images, the better the results. It is recommended that the images to align are already sharpened. Requirements:
  - the disc has to be brighter than the background
  - no vignetting or limb darkening exaggerated by post-processing
  - the disc must not be eclipsed by the Moon

Operations such as full/partial brightness inversion (creating a negative) or applying a “darkening” tone curve can be applied after the alignment.


Access by:
    menu: `Tools`/`Align image sequence...`

    
----------------------------------------
## 8. Misc

ImPPG stores certain settings (e.g. the main window’s size and position) in an INI file, whose location is platform-dependent. On recent versions of MS Windows the path is `%HOMEPATH%\AppData\Roaming\imppg.ini`, where `%HOMEPATH%` usually equals `C:\Users\<username>`. On Linux the path is `~/.imppg`.


----------------------------------------
## 9. Known problems

  - (wxGTK 3.0.2, Fedora 20) All the toolbar buttons react correctly, but their displayed state may be incorrect after using the `View` menu items/close boxes of tone curve editor and processing controls panel. Cause: sometimes check tool’s Toggle() method does not work.

  - (wxGTK) As of 2/2015, some of GTK themes function incorrectly (e.g. “QtCurve”, but not “Raleigh”). In ImPPG this may manifest as follows:
    - `File open` dialog does not show any files after it is opened; the files do appear when the dialog is resized by the user
    - program crashes when trying to select the output directory in Batch dialog using the `Choose folder` dialog
    - program hangs when trying to change file type in the `Open image file` dialog
    
Solution: change the GTK theme to "Raleigh" (e.g. in Fedora use the "GTK+ Appearance" tool).
    
  - (wxGTK) If using a GTK 3 build of wxWidgets, position of the tone curve editor’s window is not restored on startup (with GTK 2 it is).

    
----------------------------------------
## 10. Downloading

ImPPG source code and MS Windows executables can be downloaded from:  
    https://github.com/GreatAttractor/imppg/


----------------------------------------
## 11. Building from source code

Building from source code requires a C++ compiler toolchain, CMake, Boost libraries v. 1.57.0 or later (though earlier versions may work) and wxWidgets 3.0. Support for more image formats requires the FreeImage library, version 3.14.0 or newer. Without FreeImage the only supported formats are: BMP 8-, 24- and 32-bit, TIFF mono and RGB, 8 or 16 bits per channel (no compression). FITS support (optional) requires the CFITSIO library. Multithreaded processing requires compiler supporting OpenMP (e.g. GCC 4.2 or newer, MS Visual C++ 2008 or newer (non-free editions), MS Visual C++ 2012 Express or newer).

To enable/disable usage of CFITSIO/FreeImage (they are enabled by default), edit the `config.in` file.

To remove any created CMake configuration, delete `CMakeCache.txt` and the `CMakeFiles` folder.

The ImPPG executable has to be placed in the same location as the `images` and language subdirectories.

*Note: $ in the examples below indicates a console command prompt.*


### 11.1. Building under Linux and similar systems using GNU (or compatible) toolchain

*Note: CMake relies on the presence of the `wx-config` tool to detect and configure wxWidgets-related build options. Sometimes this tool can be named differently, e.g. in Fedora 23 with wxGTK3 packages from repository it is `wx-config-3.0`. This can be remedied e.g. by creating a symlink:*
```
    $ sudo ln -s /usr/bin/wx-config-3.0 /usr/bin/wx-config  
```

After making sure `wx-config` is available, execute:
```
    $ cmake -G "Unix Makefiles"
```
This creates a native `Makefile`. Unless you edit `config.in` again, from now on there is no need to run CMake.

To compile ImPPG, run:
```
    $ make
```
You will find `imppg` executable in the sources folder.


----------------------------------------
### 11.2. Building under MS Windows

As there is no standard location for development files, first you must edit `config.in` and set all the include and library paths appropriately.


#### Building with MinGW (32- or 64-bit)

Make sure MinGW toolchain is on the search path (e.g. run `set PATH=%PATH%;c:\MinGW\bin`), then run the following:
```
    $ cmake -G "MinGW Makefiles"
```
This creates a native `Makefile`. Unless you edit `config.in` again, from now on there is no need to run CMake.

To compile ImPPG, run:
```
    $ mingw32-make
```
You will find `imppg.exe` executable in the sources folder.


#### Building with Microsoft C++ (from Windows SDK or Visual Studio; 32- or 64-bit)

Make sure the environment is set up for MSVC toolchain (e.g. run `C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat` for 32-bit or `C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat` for a 64-bit build), then run the following:
```
    $ cmake -G "NMake Makefiles"
```
This creates a native `Makefile`. Unless you edit `config.in` again, from now on there is no need to run CMake.

To compile ImPPG, run:
```
    $ nmake
```
You will find `imppg.exe` executable in the sources folder.

CMake can also create MS Visual Studio project files (e.g. `cmake -G "Visual Studio 12 2013"` will create them for VS 2013). Run `cmake -G` for a list of supported generators.

----------------------------------------
### 11.3. UI language


ImPPG supports multiple user interface languages using the wxWidgets built-in internationalization facilities. All translatable strings in the source code are surrounded by the `_()` macro. Adding a new translation requires the `GNU gettext` package and consists of the following steps:

- extraction of translatable strings from sources into a PO file by running:
```
  $ xgettext -k_ *.cpp *.h -o imppg.po
```

- translation of UI strings by editing the `msgstr` entries in `imppg.po`

- converting `imppg.po` to binary form by running:
```
  $ msgfmt imppg.po -o imppg.mo
```

- placing `imppg.mo` in a subdirectory with name equal to the language code (e.g. `pl`, `fr-ca`)

- adding the language to the arrays inside `c_MainWindow::SelectLanguage()` (`main_window.cpp`)

Binary distribution of ImPPG needs only the MO (binary) language files. Beside the `imppg.mo` file(s), also the wxWidgets translation is needed (for strings like standard menu items, e.g. “Open”; control captions, e.g. “Browse” etc.). Those can be found in `<wxWidgets_source_root>/locale`. On Windows, the wxWidgets file `<language>.mo` has to be available as `<imppg_root_folder>/<language>/wxstd3.mo`. On operating systems with a common location for all `.mo` files (e.g., Linux), it is sufficient to have a wxWidgets installation.


----------------------------------------
## 12. Change log

```
0.5 (2016-01-02)
    New features:
    - Adaptive unsharp masking
    
    Enhancements:
    - Numerical sliders can be scrolled with cursor keys
    - Processing panel width is preserved
    - Using CMake for building

0.4.1 (2015-08-30)
    Enhancements:
    - Numerical sliders use 1-pixel steps instead of hard-coded 100 steps
    - Output format selected in batch processing dialog is preserved
    - Unsharp masking not slowing down for large values of *sigma*
    - Increased the range of unsharp masking parameters
    
    Bug fixes:
    - Invalid output file name after alignment if there was more than one period in input name
    - Crash when a non-existing path is entered during manual editing
    - Program windows placed outside the screen when ImPPG was previously run on multi-monitor setup
    - Restored missing Polish translation strings
    

0.4 (2015-06-21)
    New features:
    - Image sequence alignment via solar limb stabilization
    - FITS files support (load/save)
    - Zooming in/out of the view

    Enhancements:
    - View scrolling by dragging with the middle mouse button
    - Logarithmic histogram setting is preserved
    
    Bug fixes:
    - Tone curve in gamma mode not applied during batch processing

0.3.1 (2015-03-22)
    New features:
    - Polish translation; added instructions for creating additional translations

0.3 (2015-03-19)
  New features:
    - Image sequence alignment via phase correlation
    
  Enhancements:
    - Limited the frequency of processing requests to improve responsiveness during changing of unsharp masking parameters and editing of tone curve
    
  Bug fixes:
    - Incorrect output file extension after batch processing when the selected output format differs from the input

0.2 (2015-02-28)
  New features:
    - Support for more image file formats via FreeImage. New output formats: PNG/8-bit, TIFF/8-bit LZW-compressed, TIFF/16-bit ZIP-compressed, TIFF/32 bit floating-point (no compression and ZIP-compressed).
    
  Enhancements:
    - Enabled the modern-look GUI controls on Windows
    
  Bug fixes:
    - Selection border not marked on platforms w/o logical raster ops support (e.g. GTK 3)

0.1.1 (2015-02-24)
  Bug fixes:
    - Blank output files after batch processing when L–R iterations count is 0

0.1 (2015-02-21)
    Initial revision.
```