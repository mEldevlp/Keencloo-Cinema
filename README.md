<div align="center">
<h3><img src="rsc/logo_git.png" alt="logo" width="275"></h3>
</div>

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-red)
![Version](https://img.shields.io/badge/version-1.0.0-white)

### Description
**Keencloo-Cinema** is a simple and efficient video player designed to enhance the experience of movie fans.  
Key features include:
- Modern UI for browsing and playing videos
- Remember depth view video
- ~Support for drag-and-drop video loading~

### Screenshots
<details>
  <summary>View screens</summary>
  <img src="https://i.imgur.com/4ovYbzk.png" alt="first screen" width="700"/>
  <img src="https://i.imgur.com/ID9mOEy.png" alt="second screen" width="400"/>
  <img src="https://i.imgur.com/zs4hfSc.png" alt="third screen" width="400"/>
  <img src="https://i.imgur.com/pkywbwe.png" alt="fourth screen" width="400"/>
</details>


### Technologies
- **[CMake](https://cmake.org/download/)**: Latest Release (3.31.1)
- **[MSVC](https://visualstudio.microsoft.com/ru)**: Visual Studio 2022
- **[Qt](https://www.qt.io/download-qt-installer-oss)**: Version msvc 6.7.2 or higher.
- **[FFmpeg](https://github.com/BtbN/FFmpeg-Builds/releases/tag/latest)**: Build n7.1 
- **[RapidJSON](https://github.com/Tencent/rapidjson/releases/tag/v1.1.0)**: Release v1.1.0


### How to Install & Build
Follow these steps to set up and build the project:

1. Clone the repository:
   ```bash
   git clone https://github.com/mEldevlp/Keencloo-Cinema.git
   cd Keencloo-Cinema
   ```

2. Create and navigate to the `libs` directory:
   ```bash
   mkdir libs
   cd libs
   ```

3. Download and set up required dependencies:
   - Clone **RapidJSON**:
     ```bash
     git clone https://github.com/Tencent/rapidjson.git
     ```
   - Download and extract **FFmpeg**:
     ```bash
     Invoke-WebRequest -Uri "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n7.1-latest-win64-gpl-shared-7.1.zip" -OutFile "ffmpeg.zip"
     Expand-Archive -Path "ffmpeg.zip" -DestinationPath .
     Rename-Item -Path "ffmpeg-n7.1-latest-win64-gpl-shared-7.1" -NewName "ffmpeg"
     Remove-Item -Path "ffmpeg.zip" -force
     ```

4. Configure the build environment:
   ```bash
   mkdir ../build
   cd ../build
   cmake ..
   ```

5. Build the project:
   ```bash
   cmake --build . --config RelWithDebInfo
   ```

#### For debug videos use folders:
```
Keencloo-Cinema\build\Release\videos\
Keencloo-Cinema\build\RelWithDebInfo\videos\
```

### TODO
- [x] Implement basic video playback
- [ ] Add drag-and-drop support for loading videos
- [ ] Implement search and sorting in 'ExploreFilesList'
- [ ] Improve UI design with a modern look
- [ ] Context menu with options like "Open via Keencloo-Cinema"

---

### License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/mEldevlp/Keencloo-Cinema/blob/master/LICENSE) file for details.
