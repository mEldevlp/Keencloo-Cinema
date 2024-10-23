<div align="center">
<h3>Keencloo</h3>
</div>

WIP

Simplify the lives of movie fans
## Technologies
- **[Qt](https://www.qt.io/download-dev)**: Version 6.7.2 or higher.
- **[FFmpeg](https://github.com/BtbN/FFmpeg-Builds/releases/tag/latest)**: build n7.1 
- **[RapidJSON](https://rapidjson.org/)**: _(soon in project)_

#### Install & Build
Install environment variable Qt path:
Open terminal with **Admin** access
```powershell
[System.Environment]::SetEnvironmentVariable("QTDIR2", "C:\Qt\6.7.2\msvc2019_64", "Machine")
[System.Environment]::GetEnvironmentVariable("QTDIR2","Machine") -Split ";" # (optionally) you can check var like that
$path = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
$qt_lib = "%QTDIR2%/lib"
$qt_bin = "%QTDIR2%/bin"
$update_path = $path + ";" + $qt_lib + ";" + $qt_bin
[System.Environment]::SetEnvironmentVariable("Path", $update_path, "Machine")
$env:Path -split ';' # (optionally) you can paths like that
```
Download libs and build
```powershell
# Choose a folder
git clone https://github.com/mEldevlp/Keencloo-Cinema.git
cd .\Keencloo-Cinema\
mkdir libs
cd .\libs\
Invoke-WebRequest -Uri "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n7.1-latest-win64-gpl-shared-7.1.zip" -OutFile "ffmpeg.zip"
Expand-Archive -Path "ffmpeg.zip" -DestinationPath .
Rename-Item -Path "ffmpeg-n7.1-latest-win64-gpl-shared-7.1" -NewName "ffmpeg"
Remove-Item -Path "ffmpeg.zip" -force
cd ../
mkdir build
cd .\build\
cmake ..
```

#### For debug videos use folder:
```
Keencloo-Cinema\build\Release\videos\
```