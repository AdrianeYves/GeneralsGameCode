# VS2022 Compile Fixes for WorldBuilderZH

These fixes address VC6-era C++ patterns that don't compile under modern MSVC (VS2022).

## Fix 1: ScriptDialog.h — Illegal qualified names in member declaration (C4596)

**File:** `GeneralsMD/Code/Tools/WorldBuilder/include/ScriptDialog.h` lines 165-166

**Before:**
```cpp
AsciiString ScriptDialog::incrementStringNumber(const AsciiString& input);
void ScriptDialog::applySmartCopyIncrement(Script* pScr);
```

**After:**
```cpp
AsciiString incrementStringNumber(const AsciiString& input);
void applySmartCopyIncrement(Script* pScr);
```

VC6 allowed redundant class qualification inside the class body. Modern MSVC rejects it.

## Fix 2: teamsdialog.cpp — Unqualified `exception` in catch handler (C2061/C2310)

**File:** `GeneralsMD/Code/Tools/WorldBuilder/src/teamsdialog.cpp` line 804

**Before:**
```cpp
} catch(exception)  {
```

**After:**
```cpp
} catch(std::exception&)  {
```

VC6 allowed unqualified `exception` and catch-by-value. Modern MSVC requires `std::` and catch-by-reference is correct practice.

## Build Command (VS2022)

```
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32 -DRTS_BUILD_ZEROHOUR=ON -DRTS_BUILD_GENERALS=OFF -DRTS_BUILD_ZEROHOUR_TOOLS=ON -DRTS_BUILD_GENERALS_TOOLS=OFF -DRTS_BUILD_OPTION_INTERNAL=ON
cmake --build build --target z_worldbuilder --config Release
```

Output: `build/GeneralsMD/Release/WorldBuilderZH.exe`
