# OPC UA Benchmarking

This repository is a collection of some very preliminary tests for measuring the OPC UA performance using the open62541 stack. 

## Throughput

Measuring the throughput is done by using an OPC UA server which provides two methods:

* Set Package Size: Sets the size of the package which should be transferred to the client
* Get Package: Sends the previously random initialized package to the client

The client first calls the `SetPackageSize` method and then in an endless loop queries the
data from the server, i.e. as soon as the server response is recieved a new request is sent.

### Result on linklocal

This test was performed on Ubuntu 16.04 using the linklocal address. Server and client are running on the same machine.

#### Package size: **1KB (1024 Bytes)**

```
Method call was successfull, and package is set to 1 KB
Got first package. Size 1024 bytes
Rate = 165.957 Mebibit/s = 20.745 Mebibyte/s Total Size = 41MiB, Time = 2 s
Rate = 244.252 Mebibit/s = 30.531 Mebibyte/s Total Size = 122MiB, Time = 4 s
Rate = 271.861 Mebibit/s = 33.983 Mebibyte/s Total Size = 203MiB, Time = 6 s
...
Rate = 314.615 Mebibit/s = 39.327 Mebibyte/s Total Size = 2202MiB, Time = 56 s
Rate = 315.214 Mebibit/s = 39.402 Mebibyte/s Total Size = 2285MiB, Time = 58 s
Rate = 315.362 Mebibit/s = 39.420 Mebibyte/s Total Size = 2365MiB, Time = 60 s

```

Average overall transfer rate: 315Mbps

#### Package size: **1MB (1048576 Bytes)**
```
Method call was successfull, and package is set to 1024 KB
Got first package. Size 1048576 bytes
Rate = 5308.000 Mebibit/s = 663.500 Mebibyte/s Total Size = 1327MiB, Time = 2 s
Rate = 6430.000 Mebibit/s = 803.750 Mebibyte/s Total Size = 3215MiB, Time = 4 s
Rate = 6537.333 Mebibit/s = 817.167 Mebibyte/s Total Size = 4903MiB, Time = 6 s
...
Rate = 6933.143 Mebibit/s = 866.643 Mebibyte/s Total Size = 48532MiB, Time = 56 s
Rate = 6929.379 Mebibit/s = 866.172 Mebibyte/s Total Size = 50238MiB, Time = 58 s
Rate = 6936.267 Mebibit/s = 867.033 Mebibyte/s Total Size = 52022MiB, Time = 60 s
```

Average overall transfer rate: 6936Mbps


#### Package size: **100MB (104857600 Bytes)**
```
Method call was successfull, and package is set to 102400 KB
Got first package. Size 104857600 bytes
Rate = 4800.000 Mebibit/s = 600.000 Mebibyte/s Total Size = 1200MiB, Time = 2 s
Rate = 6800.000 Mebibit/s = 850.000 Mebibyte/s Total Size = 3400MiB, Time = 4 s
Rate = 7333.333 Mebibit/s = 916.667 Mebibyte/s Total Size = 5500MiB, Time = 6 s
...
Rate = 8471.429 Mebibit/s = 1058.929 Mebibyte/s Total Size = 59300MiB, Time = 56 s
Rate = 8468.966 Mebibit/s = 1058.621 Mebibyte/s Total Size = 61400MiB, Time = 58 s
Rate = 8480.000 Mebibit/s = 1060.000 Mebibyte/s Total Size = 63600MiB, Time = 60 s

```
Average overall transfer rate: 8480Mbps

### Result on local network

This test was performed on two Ubuntu 16.04 PCs which are connected through an unmanaged switch.
Server and client are running on different machines.



#### Package size: **1KB (1024 Bytes)**

```
Method call was successfull, and package is set to 1 KB
Got first package. Size 1024 bytes
Rate = 14.844 Mebibit/s = 1.855 Mebibyte/s Total Size = 3MiB, Time = 2 s
Rate = 17.754 Mebibit/s = 2.219 Mebibyte/s Total Size = 8MiB, Time = 4 s
Rate = 18.971 Mebibit/s = 2.371 Mebibyte/s Total Size = 14MiB, Time = 6 s
...
Rate = 21.012 Mebibit/s = 2.626 Mebibyte/s Total Size = 147MiB, Time = 56 s
Rate = 21.014 Mebibit/s = 2.627 Mebibyte/s Total Size = 152MiB, Time = 58 s
Rate = 21.009 Mebibit/s = 2.626 Mebibyte/s Total Size = 157MiB, Time = 60 s
```

Average overall transfer rate: 21Mbps

#### Package size: **1MB (1048576 Bytes)**
```
Method call was successfull, and package is set to 1024 KB
Got first package. Size 1048576 bytes
Rate = 716.000 Mebibit/s = 89.500 Mebibyte/s Total Size = 179MiB, Time = 2 s
Rate = 772.000 Mebibit/s = 96.500 Mebibyte/s Total Size = 386MiB, Time = 4 s
Rate = 790.667 Mebibit/s = 98.833 Mebibyte/s Total Size = 593MiB, Time = 6 s
...
Rate = 824.429 Mebibit/s = 103.054 Mebibyte/s Total Size = 5771MiB, Time = 56 s
Rate = 824.552 Mebibit/s = 103.069 Mebibyte/s Total Size = 5978MiB, Time = 58 s
Rate = 824.667 Mebibit/s = 103.083 Mebibyte/s Total Size = 6185MiB, Time = 60 s

```
Average overall transfer rate: 824Mbps

#### Package size: **100MB (104857600 Bytes)**
```
Method call was successfull, and package is set to 102400 KB
Got first package. Size 104857600 bytes
Rate = 800.000 Mebibit/s = 100.000 Mebibyte/s Total Size = 200MiB, Time = 2 s
Rate = 800.000 Mebibit/s = 100.000 Mebibyte/s Total Size = 400MiB, Time = 4 s
Rate = 800.000 Mebibit/s = 100.000 Mebibyte/s Total Size = 600MiB, Time = 6 s
...
Rate = 857.143 Mebibit/s = 107.143 Mebibyte/s Total Size = 6000MiB, Time = 56 s
Rate = 855.172 Mebibit/s = 106.897 Mebibyte/s Total Size = 6200MiB, Time = 58 s
Rate = 866.667 Mebibit/s = 108.333 Mebibyte/s Total Size = 6500MiB, Time = 60 s
```
Average overall transfer rate: 866Mbps