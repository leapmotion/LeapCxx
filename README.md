# LeapCxx
Implementation of older C++ API using LeapC

###
In Leap SDK 4.0 and beyond, the old C++ API is no longer available. To help ease the transition, we are providing
this wrapper which implements the old api (and the various SWIG based language bindings) using LeapC. This is done
in a rather quick-and-dirty way, and since the primary motivation for removing the C++ Api was that it had architectual
constraints that made it inherintly slow and difficult to maintain, this should **NOT** be taken as an example of how
to create a good langauge binding. For that, we reccomend the LeapCSharp binding in the UnityModules project that was
designed from the ground up to take advantage of LeapC's performance.
