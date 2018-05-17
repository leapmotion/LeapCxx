# LeapCxx
Implementation of LeapAPI (1.0-3.0) using LeapC

### Introduction
Leap SDK 4.0 deprecates the older LeapAPI, which was C++ natively. To help ease the transition, we are providing
this wrapper which implements the old API (with a few minor edits) and the various SWIG-based language bindings 
using LeapC. Users should be aware that the primary motivation for removing the C++ API was that it had 
architectural constraints that made it inherently slow and difficult to maintain, so this should **NOT** be taken
as an example of how to create a good language binding. For that, we recommend the referencing LeapCSharp binding 
in the UnityModules project that was designed from the ground up to take advantage of LeapC's performance.

### Migration from LeapAPI
LeapCxx is nearly identical to the old LeapAPI with a few notable exceptions:
 - The configuration API is signifigantly different
 - Several fields which had been deprecated have been removed, including gestures and the interaction box
 - Static linkage is now recommended for C++

Because this is a binding layer on top of LeapC, we recommend that users link with LeapCxx statically,
then ship only LeapC.dll with their application.

### Suggestions for future work
The current API, for the sake of compatibility, uses reference counting and a PIMPL idiom quite extensively. 
Neither of those are strictly necessary in a wrapper layer, and it should be possible to create a much faster,
lighterweight C++ and even SWIG binding layer which more directly maps to the LeapC idioms and allows a more
object oriented approach, for example allowing

```
auto connection = LeapCreateConnection(...);
connection.open();
LeapEvent event = connection.poll(...);
```
instead of
```
  LEAP_CONNECTION connection;
  LeapCreateConnection(cfg, &connection);
  LeapOpenConnection(connection);
```

Users wanting to implement such a wrapper should consult the LeapC documentation
for more information.

## Building LeapCxx and the SWIG Bindings

We use CMake, see the V4 SDK README for general information about how to use
CMake. As with any LeapC project, `LeapSDK_DIR` should be set to `<your LeapSDK
folder>/lib/cmake/LeapSDK`. For just LeapCxx, no additional configuration is
needed.

For the Java/Python bindings, SWIG needs to be installed. For Windows,
you should get it [here](http://www.swig.org/download.html). Version 3.0.12 (the
latest when this was written) is known to work. SWIG lacks a Windows installer,
so you'll need to set `SWIG_EXECUTABLE` to the path to `swig.exe`. Alternatively
you can add the SWIG folder to your `PATH`, but this isn't necessary.

To build the SWIG bindings, set `BUILD_SHARED_LIBS` and `BUILD_SWIG` to `ON`.
If you are using `cmake-gui`, you'll need to turn on `BUILD_SHARED_LIBS`,
press configure, and then `BUILD_SWIG` will be available.

### LeapPython

At least Python 3.6 is required.

### LeapJava

JDK 8 or higher is required.
