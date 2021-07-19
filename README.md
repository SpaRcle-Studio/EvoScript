# EvoScript
This library is a new scripting system based on dynamic modules (libraries), it uses C++ as a scripting language.
To compile, you must have cmake in the system environment variables, as well as a compiler, which must be specified when creating an instance of the "Compiler"class.
The library has pre-built functions:
* Awake - should be called by the engine when all scripts are run for the first time
* Start - called for each script after "Awake" and before "Update/FixedUpdate"
* Update - called as many times as the engine is able to call
* Fixed Update - called a specific number of times per second
* Close - called when the script is stopped
