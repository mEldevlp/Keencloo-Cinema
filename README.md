<div align="center">
<h3><img src="rsc/logo_git.png" alt="logo" width="275"></h3>
</div>
WIP

Simplify the lives of movie fans

### Screenshots
<details>
  <summary>View screens</summary>
  <img src="https://i.imgur.com/QMXUBdY.png" alt="first screen" width="700"/>
  <img src="https://i.imgur.com/qaDk3Qp.png" alt="second screen" width="400"/>
</details>

### Technologies
- **[Qt](https://www.qt.io/download-dev)**: Version 6.7.2 or higher.
- **[FFmpeg](https://github.com/BtbN/FFmpeg-Builds/releases/tag/latest)**: build n7.1 
- **[RapidJSON](https://rapidjson.org/)**: _(soon in project)_

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
- [ ] To develop own video widget
- [ ] Should remember the length of the viewed video, i.e. store the current viewing time somewhere
- [ ] When closing, remember in the paragraph above
- [ ] Full screen capability
- [ ] Volume control
- [x] Pause/Playback
- [ ] The video bar should be on top of the video (if possible)
- [ ] After watching 5 seconds of the video, then the video bar is hidden
- [ ] When moving the mouse while watching a video (the focus should be in the window), the video bar is displayed
- [ ] Customize the video bar
- [x] Run through a list and an abstract model
- [ ] There should be a video file name, the length of the video, and how much has already been watched, if not watched, then do not show it or highlight it separately somehow
- [ ] Hover effects when pointing at elements
- [ ] When clicked, this video opens
- [ ] Settings window
- [ ] Drag and drop video in video player

and more..
