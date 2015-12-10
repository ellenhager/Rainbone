# Setup rainbone

Rainbone uses three different frameworks, SGCT, PORTAUDIO and SFML.

----

## Download and add to local directories

### Setup SGCT
* Download the SGCT
	- The latest latest version from https://c-student.itn.liu.se/wiki/develop:sgct:download:download or clone the github repo https://github.com/opensgct/sgct
		- Current version runs SGCT 2.7

* Add include and lib to local directory
	- `lib` to `/usr/local/lib`
	- `include` to `/usr/local/include`

### Setup PORTAUDIO

#### PortAudio on OS X El Capitan

* Download [PortAudio](http://www.portaudio.com/download.html)

* Copy `portaudio.h` from the `include` folder to `usr/local/include`.

* Run `./configure && make`, if it compiles it creates a file named `libportaudio.dylib` in `usr/local/lib`.

* If it does not compile, copy the `lib` files for PORTAUDIO from the repo to `usr/local/lib`.

### Setup SFML
* Download the SFML SDK
	- The latest latest version from http://www.sfml-dev.org/download.php, that is compatible with C++11.
		- Current version runs SFML 2.3.2

* Add include, lib and Frameworks to local directory
	- `lib` to `/usr/local/lib`
	- `include` to `/usr/local/include`
	- `Frameworks` to `/Library/Frameworks`

* Add external SFML dependencies
	- `extlibs` to `/Librarys/Frameworks`

----

## Build and compile

### Build and compile on OS X El Capitan

* Open the CMake GUI

* `Browse Source`: the root folder `Rainbone`

* `Browse Build`: the `build` folder in `Rainbone`

* Check the `Grouped` checkbox

* Press `Configure`

* Choose `Unixmakefiles` in the dropdown menu

* Locate and set debug and release libraries for SGCT, PORTAUDIO and SFML
	- They should be located as below:
		- `SGCT_DEBUG_LIBRARY` at `/usr/local/lib/libsgct_cpp11d.a`
		- `SGCT_RELEASE_LIBRARY` at `/usr/local/lib/libsgct_cpp11.a`
		- `PORTAUDIO_DEBUG_LIBRARY` and `PORTAUDIO_RELEASE_LIBRARY` at `/usr/local/lib/libportaudio.dylib`
		- `SFML_AUDIO_DEBUG_LIBRARY` at `/usr/local/lib/libsfml-audio.2.3.2.dylib`
		- `SFML_AUDIO_RELEASE_LIBRARY` at `/Library/Frameworks/sfml-audio.framework`
		- `SFML_SYSTEM_DEBUG_LIBRARY` at `/usr/local/lib/libsfml-system.2.3.2.dylib`
		- `SFML_SYSTEM__RELEASE_LIBRARY` at `/Library/Frameworks/sfml-system.framework`

* Set `_INCLUDE_DIRECTORY` for SGCT, PORTAUDIO and SFML to `/usr/local/include`

* Press `Configure` again

* Press `Generate`

* A Makefile has now been generated in the `build` folder. Run `make` from the `build` folder to compile the program.

* Create your own run.command file in the `build` folder, see examples in SGCT, and set the config file to run with the program. Type `./run.command` to run the application.

