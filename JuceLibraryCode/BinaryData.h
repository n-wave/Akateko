/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_13674051_INCLUDED
#define BINARYDATA_H_13674051_INCLUDED

namespace BinaryData
{
    extern const char*   Akateko_png;
    const int            Akateko_pngSize = 2708090;

    extern const char*   AkatekoMetal_png;
    const int            AkatekoMetal_pngSize = 2152190;

    extern const char*   AkatekoSmall_png;
    const int            AkatekoSmall_pngSize = 2183616;

    extern const char*   hexagonpattern_png;
    const int            hexagonpattern_pngSize = 44815;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 4;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
