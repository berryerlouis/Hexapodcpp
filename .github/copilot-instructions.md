# Copilot Project Instructions: Hexapodcpp

Purpose: provide concise, actionable guidance for AI coding agents working in this
repo — architecture, key patterns, build/test workflows, and wiring examples.

1) Big picture
- Layered design: Drivers (`src/Driver`), Components (`src/Component`),
  Services (`src/Service`), Clusters (`src/Cluster`), Bot (`src/Bot`), Move logic
  (`src/Move`) and the composition root `src/App/App.cpp` which runs
  `Initialize()` then a light-weight periodic `Update(currentTime)` loop.

2) Core patterns (must-follow)
- Init: use `INIT("Name", obj.Initialize());` and check `Core::Status::CORE_OK`.
- Update: add services to `Service::Services` so `Update()` is called each tick.
- Observer: use `ComponentInterface<MAX_OBSERVERS, T>` (`Attach()` / `Notify()`);
  avoid dynamic allocation; prefer fixed-size arrays.
- Protocol handlers: clusters implement `Execute(Frame&, Frame&)` and
  `GetClusterId()`; use `BuildFrameNack()` for errors.
- Constraints: compiled with `-fno-exceptions` (no exceptions/RTTI); use
  explicit init lists and fixed-size containers.

3) Build & test (practical)
- Build (dev): `bin/dev/build.sh <TARGET:X64|RPI> <sources|test> <DEBUG|RELEASE>`
- Test: `bin/dev/test.sh all` or `cmake -DGTEST=1`; tests live under
  `build/*/unittests/` (e.g. `build/hexapod-X64-TEST/unittests`).
- Cross-compile: `bin/dev/docker-build-rpi.sh` or use
  `cmake/toolchain-rpi-zero-2-w.cmake` with `-DTARGET=RPI`.
- HMI: `HMI/` is a Vite app (port 8080) — run `npm run dev` inside `HMI/`.

4) Where to look (examples)
- Composition & init: `src/App/App.cpp`
- Services: `src/Service/*` (derive `ServiceInterface`)
- Components & observer usage: `src/Component/*`
- Clusters/protocol: `src/Cluster/*`
- Drivers: `src/Driver/*`

5) Adding new code (how to wire)
- New Driver: add `src/Driver/<Name>/`, implement `Initialize()`/`Update()`,
  then wire into `App` if global.
- New Component: derive `ComponentInterface<MAX_OBSERVERS, T>` when broadcasting;
  otherwise implement `Initialize()/Update()` and add to `App`.
- New Service: derive `ServiceInterface`, inject components, add to
  `Service::Services` and the `App` initializer list.
- New Cluster: implement cluster class and add its instance into `App`'s
  clusters list.

6) Quick snippets & tips
Example minimal service:
```cpp
class ServiceFoo : public Service::ServiceInterface {
 public:
  ServiceFoo(Component::Battery::Battery &b) : mBattery(b) {}
  Core::Status Initialize() override { return Core::Status::CORE_OK; }
  void Update(uint64_t) override {}
 private:
  Component::Battery::Battery &mBattery;
};
```

Practical tips: always add new sources to `src/CMakeLists.txt` or an
appropriate `add_subdirectory()`; use logging macros (`LOG_COMPONENT_INFO`,
`LOG_SERVICE_ERROR`) for consistent logs.

If you want this expanded into PR checklists (build, wiring, tests, clusters),
tell me which area to expand next.
