# mempool
A minimal memory pool library

## Build example
```
cmake --preset clang-debug
cd build/clang-debug
cmake --build .
```

## Runing tests
```ctest```
(From inside build folder)

## Using in a cmake project
```
include(FetchContent)

FetchContent_Declare(
  mempool 
  GIT_REPOSITORY https://www.github.com/mttjcksn/mempool
  GIT_TAG  main
)

FetchContent_MakeAvailable(mempool)
```

And reference source files with:

```"${mempool_SOURCE_DIR}/src"```