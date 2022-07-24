# Pthreads pool manager

POSIX threads (pthreads) pool manager written in C++. This project is a POC. The
pool manager accepts task descriptors from standard input.

## Build and execute

To build the program, simply run `make`. The executable `prog` will be
generated. To run the program, follow the format:

```
./prog <min_threads> <max_threads>
```

The program received task descriptors from standard input, in the format:

```
<TASK_ID> <PROCESSING_TIME_IN_MILISECONDS>
```

See `test/fixtures/task_descriptors.txt` for an example of task descriptors. The
script `load_gen.sh` can be used to generated load that can be pipelined to the
pool manager.

## Further documentation

Please find the documentation for the project (written in portuguese) in the
folder `docs`.
