
## MtaWorldConvertingTools

MtaWorldConvertingTools is a tool that converts maps from your favorite GTA: SA mods in [MTA](https://github.com/multitheftauto/mtasa-blue)

## Build
### Linux
* Install Qt >=6.8 and Qt Creator
* Clone the repository
* Run in terminal/powershell from repository folder
```bash
cmake -B ./build -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --config Release --target install
```
* Binary files are located in the "install" folder

### Windows
* Install Qt >=6.8 and Qt Creator
* Clone the repository
* Open the repository in Qt Creator
* Select Project -> `<your kit>` -> Build -> Build steps -> add `install` target
* Press Build -> Build All Projects
* Binary files are located in the "install" folder

## How to use
* Select original GTA: SA path
* Select modded GTA: SA path
* Select output path
* Click "Convert"
* Use [Streamer](https://github.com/TheNormalnij/Streamer) to load map

## Suppotted features
* IDE/IPL converting
* IMG repacking
* `data/object.dat` parsing. Physical objects support.
* `data/water.dat` parsing.
