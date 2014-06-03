FreeFlow
========

A OpenFlow Protocol Stack


Project Layout
--------------

$TOP/
  cmake    -- Build configuration
  freeflow -- The FreeFlow libraries
  ...


Configuration
-------------

Configuration and building the project requires CMake, and is typically
built "outside" of the source tree. My general strategy is to create a
separate build directory inside the source code tree, like this.

    > cd ${FREEFLOW_DIR}
    > mkdir build
    > cd build
    
Here, `FREEFLOW_DIR` is a placeholder for the path of the freeflow source
code directory (the one you have recently cloned).

Before building, the build system must be configured. Inside the build
directory, run the `cmake` command, giving its argument the path to freeflow's
top-level CMakeLists.txt directory. From within the build directory I
just created, I would run this:

    > cmake ..

You can use the CMake GUI to refine the build configuration, by running
the `ccmake` program, pointing it at the current build directory. I would
run this:

    > ccmake .

There are a number of CMake-specific properties that can be set:

- `CMAKE_BUILD_TYPE` -- This can be set to "Debug" or "Release" to determine
the build profile. By default, this is empty, which corresponds to a
debug build.
- `CMAKE_INSTALL_PREFIX` -- Sets the path where freeflow components are
installed. This is `/usr/local/` by default. I typically install custom
software in my home director in `~/opt`.

There are also some freeflow properties that can be configured:

- `FREEFLOW_BUILD_TESTS` -- When set, unit test are also built.

Build
-----

After the build has been configured, freeflow components can be built by
running make from within the build directory.

    > make

Installation
------------

After building the software, freeflow components can be installed by running

    > make install

Note that header header files will be installed in the `$CMAKE_INSTALL_PREFIX`
directory so that header files are placed in `include/freeflow`, libraries
in `lib`, and programs in `bin`.
