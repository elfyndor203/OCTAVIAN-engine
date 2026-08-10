# For each system:

- Create a systemDescription.c and .h
- Create a system singleton struct
- Create a system register function that describes, optionally:
  - Provided components
  - Provided public data pools
  - Provided public fields
  - Required public fields
  - An init function

- Create a CMakeLists.txt that includes:
  - A library for the engine to link against publicly
  - A system library with the systemDescriptions for the engine to link against privately


