# JsonCPP 

[open source on github](https://github.com/open-source-parsers/jsoncpp)

uses CMake to build

has separate versions for C++11 and older compilers 

1 header, 1 cpp

License: MIT

#  Casablanca

[open source on Codeplex](http://casablanca.codeplex.com/SourceControl/latest)

Depends on boost

Uses MSBuild on Windows, CMake on Android and iOS

MSVS 2013 and 2015 

#  #JSONSpirit 

[source on CodePorject](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented)

Depends on boost

# minijson_reader, minijson_writer

open source on github:

[https://github.com/giacomodrago/minijson_reader](https://github.com/giacomodrago/minijson_reader), 
[https://github.com/giacomodrago/minijson_writer](https://github.com/giacomodrago/minijson_writer)

Writer: a single header. Requires either a C++11 compliant compiler or Boost
Reader: a single header without library dependencies. C++11 is recommended but not required.

Does not use DOM-based approach (does not build the entire object in memory)

# libjson #

[open source on SourceForge](http://sourceforge.net/projects/libjson/)

Windows, Linux, OSX

# RapidJSON

[open sourceon github](https://github.com/miloyip/rapidjson)

Supports both SAX and DOM style API.

Header-only, no dependencies (not even STL)

License: MIT (not clear)

Beats all others in speed, memory usage, and compliance [https://github.com/miloyip/nativejson-benchmark](https://github.com/miloyip/nativejson-benchmark)

