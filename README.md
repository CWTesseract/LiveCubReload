# LiveCubReload
A Cube World 1.0.0-1 mod to automatically reload the `.cub` files live.

## Installation and usage
1. Download the release DLL from the [releases page](https://github.com/CWTesseract/LiveCubReload/releases), or build yourself with the instructions below.
2. Setup [ChrisMiuchiz's mod loader](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher).
3. Place the DLL in the `Mods` folder and start the game with the launcher.
4. Anytime you modify or override a `.cub` file in the `sprites` directory, it will be automatically reloaded.

## Building
1. Follow the Visual Studio 2019 with LLVM/Clang setup instructions linked in the [CWTCore](https://github.com/CWTesseract/CWTCore) readme.
2. Clone this repo with `git clone --recurse-submodules https://github.com/CWTesseract/LiveCubReload`
3. Open the folder in VS2019.
4. Build with `x64-Clang-Release`.

## Acknowledgements
This project uses the [Entropia File System Watcher (efsw)](https://bitbucket.org/SpartanJ/efsw/src/default/) library to detect changes.