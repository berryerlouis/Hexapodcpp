# Copilot Project Instructions: Hexapodcpp

These instructions capture project-specific architecture, workflows, and conventions so AI coding agents can be productive immediately. Keep answers concrete and aligned with existing patterns (do not invent new frameworks).

## 1. High-Level Architecture
The codebase models a hexapod robot with a layered design:
- **Driver** (`src/Driver/*`): Lowest-level hardware abstractions (GPIO, UART, TWI/I2C, InputCapture, Tick). Each driver implements `Initialize()` + `Update(uint64_t currentTime)` in classes derived from `DriverInterface`.
- **Component** (`src/Component/*`): Single-purpose logical units built atop drivers (Battery, Button, Servos, IMU `Mpu9150`, Barometer, Proximity sensors like `Srf05`, `Vl53l0x`, Display `Ssd1306`, Communication, Software). Components derive from `ComponentInterface<MAX_OBSERVERS, T>` enabling Observer pattern broadcasting.
- **Service** (`src/Service/*`): Coordinate multiple components to implement behaviors (Battery, Button, Control, Communication, Display, Orientation, Proximity, Sound, General). All derive from `ServiceInterface` with `Initialize()` and `Update(currentTime)`.
- **Cluster** (`src/Cluster/*`): Request/response command handling for external communication. Each cluster implements `Execute(Frame &request, Frame &response)` and `GetClusterId()`. `ClusterBase` composes a `ClusterCommand` strategy; `Clusters` aggregates active cluster instances.
- **Bot** (`src/Bot/*`): Higher-level physical model (Body, Legs) built on servos.
- **Move**: Gait and walking logic.
- **App** (`src/App/App.cpp/.h`): Composition root wiring all drivers, components, clusters, services, and bot parts. Provides global `Initialize()` then periodic `Update()` loop (calls `Services.Update()` + PWM LED update + tick delay).
- **Core**: Shared utilities (`Logger`, `Observable`, status codes, base interfaces).
- **Misc**: Helper structures (math, buffers, bitmaps, etc.).

## 2. Core Patterns & Conventions
- **Initialization Chain**: In `App::Initialize()`, failures short-circuit; use `Core::Status::CORE_OK` checks. Follow existing macro pattern: `INIT("Name", object.Initialize());` for new subsystems.
- **Update Loop**: Keep per-cycle work lightweight; new services should expose `Update(currentTime)` and be added to `Service::Services` aggregator.
- **Observer Pattern**: Use `ComponentInterface<MAX_OBSERVERS, T>` when a component must notify listeners (`Attach()` then `Notify()` protected method). Avoid dynamic allocation; fixed-size arrays via template `MAX_OBSERVERS`.
- **Clusters**: Implement protocol handlers mapping incoming frames to actions. Provide `BuildFrameNack()` for error cases; reuse existing patterns in cluster classes under `src/Cluster/*`.
- **Logging**: Use level + domain macros: `LOG_COMPONENT_INFO("Battery", "voltage=%u", v);`, `LOG_SERVICE_ERROR(...)`, `LOG_GENERIC(level, COMPONENT, ...)`. Always prefer domain macros over raw `Logger::Log` for consistency.
- **Status Codes**: Return `Core::Status` from all `Initialize()` & command executions; no exceptions (compile flags disable exceptions / RTTI).
- **Memory & Exceptions**: Build flags disable exceptions (`-fno-exceptions`) and RTTI. Do not introduce code relying on exceptions or dynamic type casting.

## 3. Build & Configuration
Primary build script: `bin/dev/build.sh` arguments:
1. Target: `X64` or `RPI`
2. Mode: `sources` or `test`
3. Build type: `DEBUG` / `RELEASE` / `CLEAN`
4. Optional: `install` (for WiringPi after fetch when `RPI`)
Script runs `prebuild.sh` then executes `make -j16` inside `build/`.

Manual CMake examples (from README):
```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET=X64 -S . -B build/gcc-debug
cmake --build build/gcc-debug --target Hexapodcpp -- -j16
```
Cross-compilation: Use Docker or `cmake/toolchain-rpi-zero-2-w.cmake` with `-DTARGET=RPI`.

## 4. Testing
- Enable tests with `-DGTEST=1` or via `bin/dev/test.sh all` (sets target X64 test build).
- Test binary path: `build/hexapodTest/unittests/HexapodcppTest`.
- Script auto-runs with flags: `--gtest_shuffle --rerun-failed --output-on-failure --gtest_output=xml:../test_results.xml`.
- Add new test directories by extending `unittests/CMakeLists.txt` (pattern: `add_subdirectory(src/cmp/...)`). Tests link against `GTest::gtest_main` & `GTest::gmock_main`.

## 5. Communication & HMI
- Websocket port: `8080` (TinyWebsockets library fetched via `FetchContent`).
- HMI UI in `HMI/` (TypeScript + Vite). Run with `npm run dev` inside `HMI/`.
- Clusters handle framed protocol messages; services may push events through Communication component (`Component::Communication`). For new message types: add a cluster, include it in `Clusters` aggregator, and ensure service / component emits relevant events.

## 6. Adding New Functionality
- **New Driver**: Create under `src/Driver/<Name>/`, derive from `DriverInterface`, implement `Initialize()` and `Update()`. Wire into `App` if globally needed.
- **New Component**: Derive from `ComponentInterface<MAX_OBSERVERS, T>` if it notifies observers; otherwise a plain class is acceptable but keep `Initialize()/Update()` signature for consistency.
- **New Service**: Derive from `ServiceInterface`; aggregate required components via constructor injection; register inside `Service::Services` container and `App` composition.
- **New Cluster**: Implement derived class (or reuse `ClusterBase`), provide `Execute()`, `GetClusterId()`, error frame builders, then add instance to `App` `mClusters` initializer list.
- **Extend App**: Add member fields following existing grouping (Drivers -> Components -> Clusters -> Services). Maintain initialization order so dependencies are constructed before use.

## 7. Coding Style & Constraints
- No exceptions / RTTI; avoid dynamic polymorphic downcasts.
- Fixed-size arrays preferred to dynamic containers for embedded determinism.
- Use explicit initialization lists for large composed objects (see `App::App()` ordering example).
- Keep per-tick operations bounded; use `Driver::Tick::Tick::GetInstance().DelayUs(100U)` pattern if adjusting loop pacing.

## 8. Deployment / Flashing
- AVR flashing script example: `bin/dev/upload.sh` (uses `avrdude` with `m1284p` over `/dev/ttyUSB0`). Adjust serial permissions and module: `sudo modprobe cp210x` then `chmod a+rw /dev/ttyUSB0`.
- Systemd service sample in README for running on RPi (`hexabot.service`).

## 9. Logging & Diagnostics
- Set global log level via `Core::Logger::SetLogLevel(...)` early if needed.
- Preserve colorized output flags (`-fdiagnostics-color` set in build script) when adding compiler invocations.

## 10. Common Pitfalls
- Forgetting to add new source directory to CMake leads to missing symbols; always extend the relevant `add_subdirectory()` or `SRC_FILES_*` accumulation.
- Not defining `TARGET` results in misconfigured FetchContent (platform-specific dependencies). Always pass `-DTARGET=RPI` or `X64`.
- Services must be updated each loop; omission from `Service::Services` aggregator means `Update()` never called.

## 11. Example: Minimal New Service
```cpp
class ServiceFoo : public Service::ServiceInterface {
public:
  ServiceFoo(Component::Battery::Battery &battery) : mBattery(battery) {}
  Core::Status Initialize() override { return Core::Status::CORE_OK; }
  void Update(const uint64_t currentTime) override { /* poll / act */ }
private:
  Component::Battery::Battery &mBattery;
};
```
Add to `App` members, construct in initializer list, and include in `Service::Services`.

---
If any section is unclear or missing (e.g., Move/Gait details, protocol frame format), request clarification and I will refine.
