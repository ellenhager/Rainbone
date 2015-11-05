# Rainbone

### How to build and compile on OS X El Capitan

* Open cmake GUI

* Press `Browse Source` and select the folder `Rainbone` (This is where the Cmakelist is located)

* Press `Browse Build` and select the `build` folder in `Rainbone` (This is where the makefile will end up)

* Check the ``Grouped`` checkbox

* Press ``Configure``

* Press the ``SGCT`` arrow and check if ``SGCT_DEBUG_LIBRARY`` is located in ``/usr/local/lib/libsgct_cpp11d.a``, and if ``SGCT_RELEASE_LIBRARY`` is located in ``/usr/local/lib/libsgct_cpp11.a``.  If they are, great! If not, change it to the ones specified.

* Press ``Configure`` again

* Press ``Generate``

* A Makefile has now been generated in the ``build`` folder. Navigate to it in terminal and type ``make``. The program should now be compiled if no errors occur.

* Navigate back one step to ``Rainbone`` and type ``./run.command`` to run the application. A window with a cube should now be opened.