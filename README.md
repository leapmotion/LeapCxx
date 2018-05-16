# LeapCxx
Implementation LeapAPI (1.0-3.0) using LeapC

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
Users of the SWIG based bindings will still need to build as a shared library using the cmake configuration
variable BUILD_SHARED_LIBS.

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

Users wanting to implement such a wrapper should consult the LeapC documentation for more information.
