# Project Guidelines

## Architecture
- Layered design: `Driver` -> `Component` -> `Service` -> `Cluster`, with `Bot` and `Move` used by services/components.
- Composition root is `src/App/App.cpp`: initialize dependencies in order, then run periodic `Update(currentTime)`.
- Keep responsibilities separated:
  - `src/Driver/`: low-level hardware/OS access.
  - `src/Component/`: stateful hardware abstractions and observables.
  - `src/Service/`: orchestration/business behavior.
  - `src/Cluster/`: protocol handlers for request/response frames.

## Build And Test
- On Windows, run project scripts through WSL.
- Main build command:
  - `wsl bash bin/dev/build.sh <TARGET:X64|RPI> <MODE:sources|test> <DEBUG|RELEASE>`
- Common commands:
  - `wsl bash bin/dev/build.sh X64 sources DEBUG`
  - `wsl bash bin/dev/test.sh all`
  - `wsl bash bin/dev/cppcheck.sh`
  - `wsl bash bin/dev/clang-tidy.sh`
  - `wsl bash bin/dev/coverage.sh`
  - `wsl bash bin/dev/docker-build-rpi.sh DEBUG`
- Unit tests are under `unittests/` and built in `build/hexapod-X64-TEST/unittests/`.
- Production build: `wsl bash bin/prod/build.sh`.

## Core Conventions
- Initialization pattern: use `INIT("Name", obj.Initialize());` and check `Core::Status::CORE_OK`.
- Service update loop: ensure services are registered so `Update(uint64_t)` is called each tick.
- Observer pattern: use `ComponentInterface<MAX_OBSERVERS, T>` and `Attach()`/`Notify()`.
- Cluster handlers: implement `Execute(Frame&, Frame&)` and `GetClusterId()`; use `BuildFrameNack()` on invalid requests.
- Logging: use layer macros (`LOG_COMPONENT_*`, `LOG_SERVICE_*`, `LOG_CLUSTER_*`) instead of ad-hoc logging.

## Constraints
- Code is compiled with `-fno-exceptions`; do not introduce exception-based flows.
- Prefer fixed-size containers and static composition; avoid dynamic allocation patterns unless already established in that area.
- Use explicit constructor initialization lists.
- When adding code, wire new modules in `src/CMakeLists.txt` or the relevant subdirectory `CMakeLists.txt`.

## HMI Notes
- Frontend is in `HMI/` (Vite + TypeScript + Three.js).
- Commands:
  - `cd HMI && npm run dev`
  - `cd HMI && npm run build`
- Keep TypeScript strictness clean (`noUnusedLocals`, `noUnusedParameters`).

## High-Value References
- Composition and wiring: `src/App/App.cpp`
- Service interface/pattern: `src/Service/ServiceInterface.h`
- Component observer base: `src/Component/ComponentInterface.h`
- Cluster protocol base: `src/Cluster/ClusterInterface.h`
- Core observer implementation: `src/Core/Observable.h`
- Build/test scripts: `bin/dev/build.sh`, `bin/dev/test.sh`
