<div align="center">
<h3><img src="rsc/logo_git.png" alt="logo" width="275"></h3>
</div>
WIP

Simplify the lives of movie fans

### Screenshots
<details>
  <summary>View screens</summary>
  <img src="https://i.imgur.com/0Pcatmq.png" alt="first screen" width="700"/>
  <img src="https://i.imgur.com/RmBgDe5.png" alt="second screen" width="400"/>
</details>

### Technologies
- **[CMake](https://cmake.org/download/)**: Latest Release (3.31.1)
- **[MSVC](https://visualstudio.microsoft.com/ru)**: Visual Studio 2022
- **[Qt](https://www.qt.io/download-qt-installer-oss)**: Version msvc 6.7.2 or higher.
- **[FFmpeg](https://github.com/BtbN/FFmpeg-Builds/releases/tag/latest)**: Build n7.1 
- **[RapidJSON](https://github.com/Tencent/rapidjson/releases/tag/v1.1.0)**: Release v1.1.0

### Install & Build
Install environment variable Qt path:
Open terminal with **Admin** access
```powershell
[System.Environment]::SetEnvironmentVariable("QTDIR", "C:\Qt\6.7.2\msvc2019_64", "Machine")
[System.Environment]::GetEnvironmentVariable("QTDIR","Machine") -Split ";" # (optionally) you can check var like that
$path = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
$qt_lib = "%QTDIR%/lib"
$qt_bin = "%QTDIR%/bin"
$update_path = $path + ";" + $qt_lib + ";" + $qt_bin
[System.Environment]::SetEnvironmentVariable("Path", $update_path, "Machine")
$env:Path -split ';' # (optionally) you can check paths like that
```
Download libs and build
```powershell
# Choose a folder
git clone https://github.com/mEldevlp/Keencloo-Cinema.git
cd .\Keencloo-Cinema\
mkdir libs
cd .\libs\

# Download rapidjson
git clone https://github.com/Tencent/rapidjson.git

# Download ffmpeg
Invoke-WebRequest -Uri "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n7.1-latest-win64-gpl-shared-7.1.zip" -OutFile "ffmpeg.zip"
Expand-Archive -Path "ffmpeg.zip" -DestinationPath .
Rename-Item -Path "ffmpeg-n7.1-latest-win64-gpl-shared-7.1" -NewName "ffmpeg"
Remove-Item -Path "ffmpeg.zip" -force

# Cmake build
cd ../
mkdir build
cd .\build\
cmake ..
cd ../
```
Build
```
cmake --build build --config RelWithDebInfo # or Release
```

#### For debug videos use folder:
```
Keencloo-Cinema\build\Release\videos\
```
## Features
- [x] The close, minimize, and full screen buttons are in the upper right corner
- [x] The "close" button should be red when pointing
- [x] There should be a logo in the middle of the window
- [x] Select the window color palette (#27c4c3, #ff2e2e, #990012, rgb: 192, 192, 192)
- [x] To develop own video widget
- [ ] Should remember the length of the viewed video, i.e. store the current viewing time somewhere
- [ ] When closing, remember in the paragraph above
- [x] Full screen capability
- [x] Volume control
- [x] Pause/Playback
- [x] The video bar should be on top of the video
- [x] After watching 3 seconds of the video, then the video bar is hidden
- [x] When moving the mouse while watching a video (the focus should be in the window), the video bar is displayed
- [x] Customize the video bar
- [x] Run through a list and an abstract model
- [ ] There should be a video file name, the length of the video, and how much has already been watched, if not watched, then do not show it or highlight it separately somehow
- [x] Hover effects at icons in main window
- [x] Hover effects when pointing at elements
- [x] When clicked, this video opens
- [ ] Settings window
- [ ] Drag and drop video in video player
- [ ] In 'ExploreFilesList' make search(text) & sort

and more..