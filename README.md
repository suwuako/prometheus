# Prometheus
A simple, silly elf linker written from scratch!!

## Building

### Introduction
Prometheus is built with [meson](https://mesonbuild.com/)! 

Ensure `meson` and `ninja` is installed on your system.

### Building
1. Get the source:
```
$ git clone git@github.com:suwuako/prometheus.git
```

2. Run Meson:
```
$ meson build
```

3. Compile the project:
```
$ meson compile -C build
```

### Testing
The executable will be produced in `build/prom`.

There is a custom target to run this executable post compilation:
```
$ meson compile -C build run
```
