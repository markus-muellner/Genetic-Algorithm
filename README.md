# Parallel Genetic Algorithm

## Compiler Requirements
* gcc version 7.3.0
* Target: x86_64-linux-gnu
* Thread model: posix

## Test Framework
* [CuTest 1.5](http://cutest.sourceforge.net/)

## Compile to shared object (.so)
```
make so
```

## Run Algorithm
```
make
./main
```

## Run Tests
```
make tests
./tests
```

## Build Image
```
docker build . -t algorithm:1.0.1
```

